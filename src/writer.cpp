// writer.cpp

#include "shared_memory_sysv.h"

int main() {
    SharedMemoryManager shm;
    if (!shm.createSharedMemory(true)) {
        return 1;
    }

    RingBuffer* buffer = shm.getRingBuffer();
    const char* message = "Hello, System V Shared Memory!";
    size_t message_length = strlen(message);

    // 写入数据到共享内存的环形缓冲区
    for (size_t i = 0; i < message_length; ++i) {
        // 循环写入数据
        buffer->buffer[buffer->tail] = message[i];
        buffer->tail = (buffer->tail + 1) % SHM_SIZE;
    }

    std::cout << "Message written to shared memory: " << message << std::endl;

    return 0;
}
