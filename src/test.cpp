#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>

int main() {
    const char* shm_name = "/test_shm";
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open failed");
        return 1;
    }
    std::cout << "shm_open succeeded!" << std::endl;
    shm_unlink(shm_name); // 清理共享内存
    return 0;
}
