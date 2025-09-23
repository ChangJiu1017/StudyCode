#ifndef __MEM_POOL_H__
#define __MEM_POOL_H__

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void   init_pool(void);               // 初始化全局与兼容结构（不用于并发路径）[SPSC设计说明]
int    register_thread(void);         // 注册当前线程，创建本地池与 SPSC 收件箱
int    unregister_thread(void);       // 线程退出时可调用，释放本线程页

void*  malloc_from_pool(size_t size); // 主路径无锁 + 无原子
void*  calloc_from_pool(size_t nmemb, size_t size);
void*  realloc_from_pool(void* ptr, size_t uNewSize);
int    free_to_pool(void* addr);      // 跨线程释放通过 SPSC 投递给所有者

size_t pool_total_bytes(void);        // 统计：已申请页总字节数（近似）
size_t thread_remote_drained(void);   // 当前线程从收件箱回捞的块计数


///////////********************  test *************************///////////////////////////
int thread_mem_pool_test();      // 简单测试例程
int run_test_gui_mix(int workers, int frames, int burst_ratio);
int run_test_spike_spsc_full(int producers, int iters);

#ifdef __cplusplus
}
#endif

#endif
