// writer.cpp

#include "shared_memory_ring_buffer.h"

int main() {
    SharedMemoryManager shm;
    if (!shm.createSharedMemory(true)) {
        return 1;
    }

    RingBuffer* buffer = shm.getRingBuffer();
    const char* message = "Hello, Shared Memory!";
    size_t message_length = strlen(message);

    for (size_t i = 0; i < message_length; ++i) {
        sem_wait(&buffer->empty_slots);  // 等待可写槽位
        pthread_spin_lock(&buffer->lock);

        buffer->buffer[buffer->tail] = message[i];
        buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;

        pthread_spin_unlock(&buffer->lock);
        sem_post(&buffer->filled_slots);  // 通知有新数据
    }

    std::cout << "Writer: Message written to shared memory." << std::endl;
    return 0;
}
