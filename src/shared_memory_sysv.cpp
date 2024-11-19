// shared_memory_sysv.cpp

#include "shared_memory_sysv.h"

SharedMemoryManager::SharedMemoryManager() : shm_id(-1), shm_addr(nullptr) {}

SharedMemoryManager::~SharedMemoryManager() {
    closeSharedMemory();
}

bool SharedMemoryManager::createSharedMemory(bool initialize) {
    // 创建共享内存
    key_t key = ftok("/tmp", 'R');  // 生成一个唯一的key值
    if (key == -1) {
        perror("ftok failed");
        return false;
    }

    // 获取共享内存标识符
    shm_id = shmget(key, sizeof(RingBuffer), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        return false;
    }

    // 映射共享内存
    shm_addr = static_cast<RingBuffer*>(shmat(shm_id, nullptr, 0));
    if (shm_addr == (void*) -1) {
        perror("shmat failed");
        return false;
    }

    // 如果是初始化操作，设置环形缓冲区
    if (initialize) {
        shm_addr->head = 0;
        shm_addr->tail = 0;
    }

    return true;
}

RingBuffer* SharedMemoryManager::getRingBuffer() {
    return shm_addr;
}

void SharedMemoryManager::closeSharedMemory() {
    if (shm_addr) {
        shmdt(shm_addr);  // 分离共享内存
        shm_addr = nullptr;
    }
    if (shm_id != -1) {
        shmctl(shm_id, IPC_RMID, nullptr);  // 删除共享内存段
        shm_id = -1;
    }
}
