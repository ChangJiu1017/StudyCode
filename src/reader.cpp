// reader.cpp

#include "shared_memory_sysv.h"

int main() {
    SharedMemoryManager shm;
    if (!shm.createSharedMemory(false)) {
        return 1;
    }

    RingBuffer* buffer = shm.getRingBuffer();
    char received[SHM_SIZE] = {0};
    size_t index = 0;

    // 读取共享内存中的数据
    while (buffer->head != buffer->tail) {  // 直到缓冲区为空
        received[index++] = buffer->buffer[buffer->head];
        buffer->head = (buffer->head + 1) % SHM_SIZE;
    }

    std::cout << "Message read from shared memory: " << received << std::endl;

    return 0;
}
