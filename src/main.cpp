#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_SIZE 1024  // 共享内存大小

int main() {
    // 创建共享内存
    key_t key = ftok("shmfile", 65);  // 生成一个唯一的key
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    int shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);  // 创建共享内存段
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    pid_t pid = fork();  // 创建子进程

    if (pid < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {  // 子进程
        // 将共享内存附加到子进程地址空间
        char *shared_memory = (char *)shmat(shmid, NULL, 0);
        if (shared_memory == (char *)-1) {
            perror("shmat");
            exit(1);
        }

        // 读取共享内存内容
        printf("子进程读取到的内容: %s\n", shared_memory);

        // 断开共享内存
        if (shmdt(shared_memory) == -1) {
            perror("shmdt");
            exit(1);
        }

        exit(0);
    } else {  // 父进程
        // 将共享内存附加到父进程地址空间
        char *shared_memory = (char *)shmat(shmid, NULL, 0);
        if (shared_memory == (char *)-1) {
            perror("shmat");
            exit(1);
        }

        // 向共享内存写入数据
        const char *message = "Hello from parent process!";
        strncpy(shared_memory, message, SHM_SIZE);
        printf("父进程已写入共享内存: %s\n", message);

        // 等待子进程结束
        wait(NULL);

        // 断开共享内存
        if (shmdt(shared_memory) == -1) {
            perror("shmdt");
            exit(1);
        }

        // 删除共享内存段
        if (shmctl(shmid, IPC_RMID, NULL) == -1) {
            perror("shmctl");
            exit(1);
        }

        printf("共享内存已删除\n");
    }

    return 0;
}
