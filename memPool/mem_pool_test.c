#include "memPool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

static double now_sec() {
    struct timeval tv; gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

static void test_init_basic() {
    printf("[TEST] init + register\n");
    sleep(10);
    init_pool();
    int id = register_thread();
    assert(id >= 0);
    void* p = malloc_from_pool(64);
    assert(p);
    free_to_pool(p);
    printf("  ok\n");
}

static void test_calloc_zero() {
    printf("[TEST] calloc zero\n");
    register_thread();
    void* p = calloc_from_pool(128, 1);
    assert(p);
    for (int i=0;i<128;++i) assert(((unsigned char*)p)[i]==0);
    free_to_pool(p);
    printf("  ok\n");
}

static void test_realloc_paths() {
    printf("[TEST] realloc paths\n");
    register_thread();
    void* a = malloc_from_pool(100);
    void* a2 = realloc_from_pool(a, 80);
    assert(a2 == a);
    void* a3 = realloc_from_pool(a2, 2000);
    assert(a3);
    free_to_pool(a3);
    printf("  ok\n");
}

typedef struct { void* p; } ctx_t;
static void* worker_free(void* arg) {
    register_thread();
    ctx_t* c = (ctx_t*)arg;
    free_to_pool(c->p);
    return NULL;
}

static void test_cross_thread_free() {
    printf("[TEST] cross-thread free via SPSC inbox\n");
    register_thread();
    void* p = malloc_from_pool(128);
    pthread_t th;
    ctx_t c = { .p = p };
    pthread_create(&th, NULL, worker_free, &c);
    pthread_join(th, NULL);
    void* q = malloc_from_pool(64); // 触发回捞
    assert(q);
    free_to_pool(q);
    printf("  drained=%zu\n", thread_remote_drained());
    printf("  ok\n");
}

typedef struct { int id, iters; } mt_arg;
static inline int rnd(int m){ return rand()%m; }

static void* mt_worker(void* a0) {
    mt_arg* a = (mt_arg*)a0;
    register_thread();
    for (int i=0;i<a->iters;++i) {
        size_t sz = (rnd(3)==0) ? 900 : (size_t)(rnd(200)+1);
        void* p = malloc_from_pool(sz<=1024?sz:1200);
        if (p) {
            if (rnd(2)==0) {
                free_to_pool(p);
            } else {
                size_t ns = rnd(1500);
                void* r = realloc_from_pool(p, ns);
                free_to_pool(r);
            }
        }
    }
    return NULL;
}

static void test_multithread_stress(int ths, int iters) {
    printf("[TEST] multithread stress th=%d iters=%d\n", ths, iters);
    pthread_t th[ths];
    mt_arg a[ths];
    double t0 = now_sec();
    for (int i=0;i<ths;++i) {
        a[i].id=i; a[i].iters=iters;
        pthread_create(&th[i], NULL, mt_worker, &a[i]);
    }
    for (int i=0;i<ths;++i) pthread_join(th[i], NULL);
    double t1 = now_sec();
    printf("  total_bytes=%zu time=%.3fs\n", pool_total_bytes(), t1-t0);
    printf("  ok\n");
}

int thread_mem_pool_test() {
    srand((unsigned)time(NULL));
    test_init_basic();
    test_calloc_zero();
    test_realloc_paths();
    test_cross_thread_free();
    test_multithread_stress(4, 100000);
    return 0;
}
