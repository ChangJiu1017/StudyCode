#define _GNU_SOURCE
#include "memPool.h"
#include "ring_buffer.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <malloc.h>

#ifndef CACHELINE
#define CACHELINE 64
#endif

// 日志控制：默认开启，编译时可用 -DMP_ENABLE_INFO=0 / -DMP_ENABLE_DEBUG=0 关闭
#ifndef MP_ENABLE_INFO
#define MP_ENABLE_INFO 0
#endif
#ifndef MP_ENABLE_DEBUG
#define MP_ENABLE_DEBUG 0
#endif
#ifndef MP_ENABLE_WARN
#define MP_ENABLE_WARN 0
#endif

// 线程 id 简易打印
static inline unsigned long mp_tid(void) { return (unsigned long)pthread_self(); }

// 安全的可变参日志宏
#if MP_ENABLE_INFO
  #define MP_INFO(fmt, ...)  do { fprintf(stderr, "[I][tid=%lu] " fmt "\n", mp_tid(), ##__VA_ARGS__); } while(0)
#else
  #define MP_INFO(fmt, ...)  do{}while(0)
#endif

#if MP_ENABLE_DEBUG
  #define MP_DEBUG(fmt, ...) do { fprintf(stderr, "[D][tid=%lu] " fmt "\n", mp_tid(), ##__VA_ARGS__); } while(0)
#else
  #define MP_DEBUG(fmt, ...) do{}while(0)
#endif

#if MP_ENABLE_WARN
#define MP_WARN(fmt, ...)    do { fprintf(stderr, "[W][tid=%lu] " fmt "\n", mp_tid(), ##__VA_ARGS__); } while(0)
#else
  #define MP_WARN(fmt, ...) do{}while(0)
#endif

#define BLOCK_SIZE   1024
#define PAGE_SIZE    (BLOCK_SIZE*4096)
#define SPSC_POW2    12
#define MAX_THREADS  64
#define BLOCK_NUM    10000

// ---------- SPSC 队列（严格单生产者/单消费者，无原子） ----------
typedef struct spsc_queue {
    __attribute__((aligned(CACHELINE))) volatile uint32_t head; // consumer 写
    __attribute__((aligned(CACHELINE))) volatile uint32_t tail; // producer 写
    uint32_t cap_mask;
    void**   buf;
} spsc_queue;

static inline void spsc_barrier(void) { __asm__ __volatile__("" ::: "memory"); }

static inline int spsc_init(spsc_queue* q, uint32_t pow2_cap) {
    if (!q) return -1;
    uint32_t cap = 1u << pow2_cap;
    q->buf = (void**)calloc(cap, sizeof(void*));
    if (!q->buf) return -1;
    q->head = 0;
    q->tail = 0;
    q->cap_mask = cap - 1u;
    MP_INFO("SPSC init cap=%u", cap);
    return 0;
}

static inline int spsc_enqueue(spsc_queue* q, void* p) {
    uint32_t tail = q->tail;
    uint32_t next = (tail + 1u) & q->cap_mask;
    if (next == q->head) {
        MP_DEBUG("SPSC enqueue FULL head=%u tail=%u", (unsigned)q->head, (unsigned)tail);
        return 0;
    }
    q->buf[tail] = p;   // 先写数据
    spsc_barrier();     // 屏障
    q->tail = next;     // 再发布 tail
    MP_DEBUG("SPSC enqueue ok new_tail=%u", (unsigned)next);
    return 1;
}

static inline void* spsc_dequeue(spsc_queue* q) {
    uint32_t head = q->head;
    if (head == q->tail) {
        MP_DEBUG("SPSC dequeue EMPTY head=%u", (unsigned)head);
        return NULL;
    }
    void* p = q->buf[head];
    spsc_barrier();
    q->head = (head + 1u) & q->cap_mask;
    MP_DEBUG("SPSC dequeue ok new_head=%u", (unsigned)q->head);
    return p;
}
// ----------------------------------------------------------

// 兼容保留（不参与并发）
typedef struct init_node {
    unsigned long total_size;
    uint8_t*      cur_base;
    uint8_t*      mem_base;
    ring_buffer   rb;
} init_node;

static init_node g_initNode;

// 16B 对齐块头 + 魔数
typedef struct __attribute__((aligned(16))) block {
    struct block* next;
    int           owner_tid;
    uint32_t      magic;
} block;

#define BLOCK_MAGIC 0xC0FFEEAAu

// 页
typedef struct page {
    uint8_t* base;
    uint8_t* curr;
    uint8_t* end;
    struct page* next;
} page;

// 线程本地池
typedef struct thread_pool {
    int     tid;
    block*  local_free;
    page*   cur_page;
    page*   all_pages;
    size_t  remote_drained;
    spsc_queue inbox_from[MAX_THREADS]; // producer=src, consumer=this
    int     inbox_ready[MAX_THREADS];
    block*  deferred;
} thread_pool;

// TLS（pthread）
static pthread_key_t g_tls_key;
static int g_tls_key_inited = 0;

static void tls_init_once(void) {
    if (!g_tls_key_inited) {
        int rc = pthread_key_create(&g_tls_key, /*destructor=*/NULL);
        (void)rc;
        g_tls_key_inited = 1;
        MP_INFO("TLS key created");
    }
}
static inline thread_pool* get_tls_pool(void) {
    if (!g_tls_key_inited) tls_init_once();
    return (thread_pool*)pthread_getspecific(g_tls_key);
}
static inline void set_tls_pool(thread_pool* tp) {
    if (!g_tls_key_inited) tls_init_once();
    pthread_setspecific(g_tls_key, tp);
}

// 全局线程表
static thread_pool* g_threads[MAX_THREADS];
static int          g_thread_count = 0;

// 页分配
static page* alloc_page(size_t size) {
    size_t align = 4096;
    size_t req   = (size + align - 1) / align * align;
    void* mem = NULL;
#if defined(_POSIX_VERSION)
    if (posix_memalign(&mem, align, req) != 0) mem = NULL;
#else
    mem = memalign(align, req);
#endif
    if (!mem) return NULL;
    page* pg = (page*)malloc(sizeof(page));
    if (!pg) { free(mem); return NULL; }
    pg->base = (uint8_t*)mem;
    pg->curr = pg->base;
    pg->end  = pg->base + req;
    pg->next = NULL;
    MP_INFO("alloc_page size=%zu req=%zu base=%p end=%p",
            size, req, (void*)pg->base, (void*)pg->end);
    return pg;
}

// 指针是否在某所有者的页范围中
static int is_in_any_page_owner(thread_pool* owner_tp, uint8_t* p) {
    if (!owner_tp) return 0;
    page* pg = owner_tp->all_pages;
    while (pg) {
        if (p >= pg->base && p < pg->end) return 1;
        pg = pg->next;
    }
    return 0;
}

int register_thread(void) {
    thread_pool* tls = get_tls_pool();
    if (tls) {
        MP_INFO("register_thread: already tid=%d", tls->tid);
        return tls->tid;
    }

    int tid = g_thread_count;
    assert(tid < MAX_THREADS && "too many threads");
    thread_pool* tp = (thread_pool*)calloc(1, sizeof(thread_pool));
    assert(tp);
    tp->tid = tid;
    tp->local_free = NULL;
    tp->cur_page = alloc_page(PAGE_SIZE);
    assert(tp->cur_page);
    tp->all_pages = tp->cur_page;
    tp->remote_drained = 0;
    tp->deferred = NULL;
    for (int i = 0; i < MAX_THREADS; ++i) tp->inbox_ready[i] = 0;

    g_threads[tid] = tp;
    g_thread_count++;
    MP_INFO("register_thread: tid=%d total=%d", tid, g_thread_count);

    for (int src = 0; src < g_thread_count; ++src) {
        if (src == tid) continue;
        spsc_init(&tp->inbox_from[src], SPSC_POW2);
        tp->inbox_ready[src] = 1;
        MP_DEBUG("inbox ready: consumer=%d src=%d", tid, src);
    }

    set_tls_pool(tp);
    return tid;
}

int unregister_thread(void) {
    thread_pool* tp = get_tls_pool();
    if (!tp) return 0;

    MP_INFO("unregister_thread: tid=%d", tp->tid);
    page* pg = tp->all_pages;
    size_t total = 0;
    while (pg) {
        page* nx = pg->next;
        total += (size_t)(pg->end - pg->base);
        free(pg->base);
        free(pg);
        pg = nx;
    }
    MP_INFO("unregister_thread: freed_pages_bytes=%zu", total);

    tp->deferred = NULL;
    g_threads[tp->tid] = NULL;

    set_tls_pool(NULL);
    free(tp);
    return 0;
}

void init_pool(void) {
    tls_init_once();
    g_initNode.total_size = BLOCK_SIZE * BLOCK_NUM;
    g_initNode.mem_base = (uint8_t*)memalign(0x1000, BLOCK_SIZE * BLOCK_NUM);
    g_initNode.cur_base = g_initNode.mem_base;
    uint64_t* meta = (uint64_t*)memalign(0x1000, sizeof(void*) * BLOCK_NUM);
    RB_Init(&g_initNode.rb, meta, BLOCK_NUM);
    MP_INFO("init_pool: compat total=%lu base=%p",
            g_initNode.total_size, (void*)g_initNode.mem_base);
}

// bump 分配（写魔数）
static void* bump_alloc_block(thread_pool* tp) {
    size_t need = sizeof(block) + BLOCK_SIZE;
    if ((size_t)(tp->cur_page->end - tp->cur_page->curr) < need) {
        page* np = alloc_page(PAGE_SIZE);
        if (!np) return NULL;
        np->next = tp->all_pages;
        tp->all_pages = np;
        tp->cur_page = np;
        MP_INFO("page rotate: tid=%d new_base=%p", tp->tid, (void*)np->base);
    }
    uint8_t* p = tp->cur_page->curr;
    tp->cur_page->curr += need;
    block* b = (block*)p;
    b->owner_tid = tp->tid;
    b->next = NULL;
    b->magic = BLOCK_MAGIC;
    MP_DEBUG("bump alloc: tid=%d blk=%p payload=%p need=%zu",
             tp->tid, (void*)b, (void*)(p + sizeof(block)), need);
    return (void*)(p + sizeof(block));
}

// 回捞（带防御）
static void drain_inboxes(thread_pool* tp) {
    size_t pulled = 0;
    for (int src = 0; src < g_thread_count; ++src) {
        if (src == tp->tid) continue;
        if (!tp->inbox_ready[src]) continue;
        spsc_queue* q = &tp->inbox_from[src];
        for (;;) {
            void* ptr = spsc_dequeue(q);
            if (!ptr) break;
            uint8_t* ua = (uint8_t*)ptr;

            if (!is_in_any_page_owner(tp, ua)) {
                MP_WARN("drain non-owner ptr=%p src=%d", ptr, src);
                continue;
            }
            block* b = (block*)(ua - sizeof(block));
            if (b->magic != BLOCK_MAGIC) {
                MP_WARN("drain bad magic ptr=%p src=%d", ptr, src);
                continue;
            }
            b->next = tp->local_free;
            tp->local_free = b;
            tp->remote_drained++;
            pulled++;
        }
    }
    while (tp->deferred) {
        block* b = tp->deferred;
        tp->deferred = b->next;
        if (b->magic == BLOCK_MAGIC) {
            b->next = tp->local_free;
            tp->local_free = b;
            pulled++;
        } else {
            MP_WARN("deferred bad magic b=%p", (void*)b);
        }
    }
    if (pulled) MP_INFO("drain: tid=%d pulled=%zu drained_total=%zu",
                        tp->tid, pulled, tp->remote_drained);
}

void* malloc_from_pool(size_t size) {
    thread_pool* tp = get_tls_pool();
    if (!tp) {
        register_thread();
        tp = get_tls_pool();
    }

    MP_DEBUG("malloc_from_pool enter size=%zu tid=%d", size, tp->tid);

    if (size == 0) return NULL;
    if (size > BLOCK_SIZE) {
        void* p = malloc(size);
        MP_DEBUG("malloc_from_pool sys path size=%zu -> %p", size, p);
        return p;
    }

    if (tp->local_free) {
        block* b = tp->local_free;
        tp->local_free = b->next;
        void* payload = (void*)((uint8_t*)b + sizeof(block));
        MP_DEBUG("malloc_from_pool local_free pop b=%p payload=%p", (void*)b, payload);
        return payload;
    }
    drain_inboxes(tp);
    if (tp->local_free) {
        block* b = tp->local_free;
        tp->local_free = b->next;
        void* payload = (void*)((uint8_t*)b + sizeof(block));
        MP_DEBUG("malloc_from_pool after drain pop b=%p payload=%p", (void*)b, payload);
        return payload;
    }
    void* p = bump_alloc_block(tp);
    MP_DEBUG("malloc_from_pool bump -> %p", p);
    return p;
}

void* calloc_from_pool(size_t nmemb, size_t size) {
    size_t total = nmemb * size;
    MP_DEBUG("calloc_from_pool total=%zu", total);
    if (total == 0) return NULL;
    if (total > BLOCK_SIZE) {
        void* p = calloc(nmemb, size);
        MP_DEBUG("calloc_from_pool sys path -> %p", p);
        return p;
    }
    void* p = malloc_from_pool(total);
    if (p) memset(p, 0, total);
    MP_DEBUG("calloc_from_pool pool path -> %p", p);
    return p;
}

void* realloc_from_pool(void* ptr, size_t uNewSize) {
    MP_DEBUG("realloc_from_pool ptr=%p new=%zu", ptr, uNewSize);
    if (!ptr) {
        if (uNewSize == 0) return NULL;
        if (uNewSize <= BLOCK_SIZE) return malloc_from_pool(uNewSize);
        return realloc(NULL, uNewSize);
    }
    if (uNewSize == 0) {
        free_to_pool(ptr);
        return NULL;
    }
    if (uNewSize <= BLOCK_SIZE) {
        MP_DEBUG("realloc_from_pool inplace ptr=%p", ptr);
        return ptr;
    } else {
        void* np = malloc(uNewSize);
        if (!np) return NULL;
        memcpy(np, ptr, BLOCK_SIZE);
        free_to_pool(ptr);
        MP_DEBUG("realloc_from_pool migrate old=%p new=%p", ptr, np);
        return np;
    }
}

int free_to_pool(void* addr) {
    MP_DEBUG("free_to_pool addr=%p", addr);
    if (!addr) return 0;
    thread_pool* tp = get_tls_pool();
    if (!tp) {
        register_thread();
        tp = get_tls_pool();
    }

    uint8_t* ua = (uint8_t*)addr;
    block* bhead = (block*)(ua - sizeof(block));
    int owner = bhead->owner_tid;

    thread_pool* owner_tp = (owner >= 0 && owner < MAX_THREADS) ? g_threads[owner] : NULL;
    if (!owner_tp) {
        MP_WARN("owner missing owner=%d ptr=%p -> system free", owner, addr);
        free(addr);
        return 0;
    }

    if (!is_in_any_page_owner(owner_tp, ua)) {
        MP_WARN("non-owner ptr free_to_pool ptr=%p owner=%d -> system free", addr, owner);
        free(addr);
        return 0;
    }

    block* b = (block*)(ua - sizeof(block));
    if (b->magic != BLOCK_MAGIC) {
        MP_WARN("bad magic free_to_pool ptr=%p owner=%d -> system free", addr, owner);
        free(addr);
        return 0;
    }

    if (owner == tp->tid) {
        b->next = tp->local_free;
        tp->local_free = b;
        MP_DEBUG("free_to_pool local owner tid=%d b=%p", tp->tid, (void*)b);
        return 0;
    }

    spsc_queue* q = &owner_tp->inbox_from[tp->tid];
    if (!owner_tp->inbox_ready[tp->tid]) {
        MP_INFO("inbox not ready owner=%d src=%d -> deferred", owner, tp->tid);
        b->next = tp->deferred;
        tp->deferred = b;
        return 0;
    }
    if (!spsc_enqueue(q, addr)) {
        MP_INFO("inbox full owner=%d src=%d -> deferred", owner, tp->tid);
        b->next = tp->deferred;
        tp->deferred = b;
    } else {
        MP_DEBUG("free_to_pool enqueued owner=%d src=%d payload=%p", owner, tp->tid, addr);
    }
    return 0;
}

size_t pool_total_bytes(void) {
    size_t total = 0;
    for (int i = 0; i < g_thread_count; ++i) {
        thread_pool* tp = g_threads[i];
        if (!tp) continue;
        page* pg = tp->all_pages;
        while (pg) {
            total += (size_t)(pg->end - pg->base);
            pg = pg->next;
        }
    }
    MP_DEBUG("pool_total_bytes=%zu", total);
    return total;
}

size_t thread_remote_drained(void) {
    thread_pool* tp = get_tls_pool();
    size_t v = tp ? tp->remote_drained : 0;
    MP_DEBUG("thread_remote_drained v=%zu", v);
    return v;
}
