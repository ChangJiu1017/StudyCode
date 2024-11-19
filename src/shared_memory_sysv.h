// shared_memory_sysv.h

#ifndef SHARED_MEMORY_SYSV_H
#define SHARED_MEMORY_SYSV_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

#define SHM_SIZE 1024  // 环形缓冲区的大小

// 环形缓冲区结构
struct RingBuffer {
    size_t head;
    size_t tail;
    char buffer[SHM_SIZE];  // 数据缓冲区
};

class SharedMemoryManager {
public:
    SharedMemoryManager();
    ~SharedMemoryManager();
    bool createSharedMemory(bool initialize);
    RingBuffer* getRingBuffer();
    void closeSharedMemory();

private:
    int shm_id;      // 共享内存ID
    RingBuffer* shm_addr;  // 映射到进程的共享内存地址
};

#endif // SHARED_MEMORY_SYSV_H
