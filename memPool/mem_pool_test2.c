// test_gui_mix.c
#include "memPool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

typedef struct {
    int id;
    int frames;         // 模拟帧数
    int workers;
    int burst_ratio;    // 后台线程突发释放比例(0-100)
} gui_conf;

typedef struct {
    int id;
    int iters;
    int to_owner_tid;   // 将释放集中回主线程以压满其 SPSC
} worker_conf;

static double now_sec() {
    struct timeval tv; gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

static void* worker_fn(void* arg) {
    worker_conf* c = (worker_conf*)arg;
    register_thread();
    // 大量分配，然后跨线程释放给主线程（假设主线程tid=0，或先通过接口拿到）
    // 这里简化：直接 free_to_pool 即按 owner 路由，不需要指定目标
    void** stash = (void**)calloc(4096, sizeof(void*));
    assert(stash);
    for (int i = 0; i < c->iters; ++i) {
        size_t sz = (rand() % 2) ? (size_t)(rand() % 256 + 1) : 900;
        void* p = malloc_from_pool(sz <= 1024 ? sz : 1400);
        if (p) {
            // 写入模拟像素数据
            memset(p, (unsigned char)(c->id), (sz <= 1024 ? sz : 1024));
            // 随机暂存
            int slot = rand() % 4096;
            if (stash[slot]) free_to_pool(stash[slot]);
            stash[slot] = p;
        }
        // 偶发突发释放
        if ((i % 1000) == 0) {
            for (int k = 0; k < 512; ++k) {
                int slot = rand() % 4096;
                if (stash[slot]) {
                    free_to_pool(stash[slot]);
                    stash[slot] = NULL;
                }
            }
        }
    }
    // 清空
    for (int i = 0; i < 4096; ++i) if (stash[i]) free_to_pool(stash[i]);
    free(stash);
    return NULL;
}

int run_test_gui_mix(int workers, int frames, int burst_ratio) {
    srand((unsigned)time(NULL));
    printf("[GUI] start mix: workers=%d frames=%d burst=%d%%\n", workers, frames, burst_ratio);
    init_pool();
    int main_tid = register_thread(); // GUI 主线程
    assert(main_tid == 0);

    pthread_t* th = (pthread_t*)calloc(workers, sizeof(pthread_t));
    worker_conf* wc = (worker_conf*)calloc(workers, sizeof(worker_conf));
    for (int i = 0; i < workers; ++i) {
        wc[i].id = i + 1;
        wc[i].iters = 200000;
        wc[i].to_owner_tid = 0;
        pthread_create(&th[i], NULL, worker_fn, &wc[i]);
    }

    // 主线程模拟帧循环：分配临时 UI/纹理数据，周期性 batch 释放
    void** frame_stash = (void**)calloc(2048, sizeof(void*));
    double t0 = now_sec();
    for (int f = 0; f < frames; ++f) {
        // 每帧分配一批小对象
        for (int i = 0; i < 2000; ++i) {
            size_t sz = (size_t)(rand() % 256 + 1);
            void* p = malloc_from_pool(sz);
            assert(p);
            int slot = rand() % 2048;
            if (frame_stash[slot]) free_to_pool(frame_stash[slot]);
            frame_stash[slot] = p;
        }
        // 模拟帧处理时间
        usleep(16000);

        // 每隔数帧做一次批量释放，触发回捞
        if ((f % 5) == 0) {
            for (int k = 0; k < 512; ++k) {
                int slot = rand() % 2048;
                if (frame_stash[slot]) {
                    free_to_pool(frame_stash[slot]);
                    frame_stash[slot] = NULL;
                }
            }
        }
        if ((f % 60) == 0) {
            printf("  [GUI] frame=%d drained=%zu total_bytes=%zu\n",
                   f, thread_remote_drained(), pool_total_bytes());
        }
    }

    for (int i = 0; i < 2048; ++i) if (frame_stash[i]) free_to_pool(frame_stash[i]);
    free(frame_stash);

    for (int i = 0; i < workers; ++i) pthread_join(th[i], NULL);
    free(th); free(wc);

    double t1 = now_sec();
    printf("[GUI] done time=%.3fs total_bytes=%zu drained=%zu\n",
           t1 - t0, pool_total_bytes(), thread_remote_drained());
    return 0;
}
