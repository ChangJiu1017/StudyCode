// test_spike_spsc_full.c
#include "memPool.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

typedef struct { int id, iters; } releaser_arg;

static void* releaser_fn(void* a0) {
    releaser_arg* a = (releaser_arg*)a0;
    register_thread();
    // 不断分配并立即释放（跨线程路由给 owner=0）
    for (int i = 0; i < a->iters; ++i) {
        void* p = malloc_from_pool(128);
        if (p) free_to_pool(p);
        if ((i % 100000) == 0) usleep(1000); // 微休降低完全压死消费者
    }
    return NULL;
}

int run_test_spike_spsc_full(int producers, int iters) {
    srand((unsigned)time(NULL));
    init_pool();
    int owner = register_thread(); // tid=0
    assert(owner == 0);

    pthread_t* th = (pthread_t*)calloc(producers, sizeof(pthread_t));
    releaser_arg* ra = (releaser_arg*)calloc(producers, sizeof(releaser_arg));
    for (int i = 0; i < producers; ++i) {
        ra[i].id = i + 1; ra[i].iters = iters;
        pthread_create(&th[i], NULL, releaser_fn, &ra[i]);
    }

    // owner 线程模拟“较慢消费”：每隔一段 drain 一次
    for (int round = 0; round < 200; ++round) {
        // 分配一些对象制造交叉
        for (int k = 0; k < 5000; ++k) {
            void* p = malloc_from_pool(64);
            free_to_pool(p);
        }
        usleep(20000);
        printf("[SPSC] round=%d drained=%zu total_bytes=%zu\n",
               round, thread_remote_drained(), pool_total_bytes());
    }

    for (int i = 0; i < producers; ++i) pthread_join(th[i], NULL);
    free(th); free(ra);
    return 0;
}
