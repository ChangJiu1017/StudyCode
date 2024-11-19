// shared_memory_ring_buffer.h

#ifndef SHARED_MEMORY_RING_BUFFER_H
#define SHARED_MEMORY_RING_BUFFER_H

#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define SHM_NAME "/shared_ring_buffer"
#define BUFFER_SIZE 1024  // 环形缓冲区大小

// 环形缓冲区结构
struct RingBuffer {
    pthread_spinlock_t lock;  // 自旋锁
    sem_t empty_slots;        // 可写槽位信号量
    sem_t filled_slots;       // 可读槽位信号量
    size_t head;              // 读指针
    size_t tail;              // 写指针
    char buffer[BUFFER_SIZE]; // 数据缓冲区
};

class SharedMemoryManager {
public:
    SharedMemoryManager();
    ~SharedMemoryManager();
    bool createSharedMemory(bool initialize);
    RingBuffer* getRingBuffer();
    void closeSharedMemory();

private:
    int shm_fd;
    RingBuffer* ring_buffer;
    bool is_initialized;
};

#endif // SHARED_MEMORY_RING_BUFFER_H
