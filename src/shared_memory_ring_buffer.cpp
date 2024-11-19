// shared_memory_ring_buffer.cpp

#include "shared_memory_ring_buffer.h"

SharedMemoryManager::SharedMemoryManager() : shm_fd(-1), ring_buffer(nullptr), is_initialized(false) {}

SharedMemoryManager::~SharedMemoryManager() {
    closeSharedMemory();
}

bool SharedMemoryManager::createSharedMemory(bool initialize) {
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open failed");
        return false;
    }

    if (initialize) {
        if (ftruncate(shm_fd, sizeof(RingBuffer)) != 0) {
            perror("ftruncate failed");
            return false;
        }
    }

    void* addr = mmap(nullptr, sizeof(RingBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) {
        perror("mmap failed");
        return false;
    }

    ring_buffer = static_cast<RingBuffer*>(addr);

    if (initialize) {
        pthread_spin_init(&ring_buffer->lock, PTHREAD_PROCESS_SHARED);
        sem_init(&ring_buffer->empty_slots, 1, BUFFER_SIZE);
        sem_init(&ring_buffer->filled_slots, 1, 0);
        ring_buffer->head = 0;
        ring_buffer->tail = 0;
        is_initialized = true;
    }

    return true;
}

RingBuffer* SharedMemoryManager::getRingBuffer() {
    return ring_buffer;
}

void SharedMemoryManager::closeSharedMemory() {
    if (ring_buffer) {
        munmap(ring_buffer, sizeof(RingBuffer));
        ring_buffer = nullptr;
    }
    if (shm_fd >= 0) {
        close(shm_fd);
        shm_fd = -1;
    }
}
