#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main() {
    int pipe1[2]; // 管道1: 父进程 -> 子进程
    int pipe2[2]; // 管道2: 子进程 -> 父进程

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { 
        // 子进程
        close(pipe1[1]); // 关闭管道1的写端
        close(pipe2[0]); // 关闭管道2的读端

        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;

        // 从父进程接收消息
        bytesRead = read(pipe1[0], buffer, BUFFER_SIZE - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            printf("子进程接收到消息: %s\n", buffer);

            // 回复父进程
            const char *response = "子进程已收到消息";
            write(pipe2[1], response, strlen(response));
        }

        close(pipe1[0]); // 关闭管道1的读端
        close(pipe2[1]); // 关闭管道2的写端
    } else { 
        // 父进程
        close(pipe1[0]); // 关闭管道1的读端
        close(pipe2[1]); // 关闭管道2的写端

        const char *message = "父进程的消息";
        printf("父进程发送消息: %s\n", message);
        write(pipe1[1], message, strlen(message));

        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;

        // 从子进程接收回复
        bytesRead = read(pipe2[0], buffer, BUFFER_SIZE - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            printf("父进程接收到回复: %s\n", buffer);
        }

        close(pipe1[1]); // 关闭管道1的写端
        close(pipe2[0]); // 关闭管道2的读端
    }

    return 0;
}
