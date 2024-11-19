// reader.cpp

#include "shared_memory_ring_buffer.h"

int main() {
    SharedMemoryManager shm;
    if (!shm.createSharedMemory(false)) {
        return 1;
    }

    RingBuffer* buffer = shm.getRingBuffer();
    char received[BUFFER_SIZE] = {0};
    size_t index = 0;

    for (size_t i = 0; i < BUFFER_SIZE; ++i) {
        sem_wait(&buffer->filled_slots);  // 等待有数据
        pthread_spin_lock(&buffer->lock);

        received[index++] = buffer->buffer[buffer->head];
        buffer->head = (buffer->head + 1) % BUFFER_SIZE;

        pthread_spin_unlock(&buffer->lock);
        sem_post(&buffer->empty_slots);  // 通知释放槽位
    }

    std::cout << "Reader: Received message: " << received << std::endl;
    return 0;
}
