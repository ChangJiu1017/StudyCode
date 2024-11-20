#include <iostream>
#include <mqueue.h>
#include <cstring>
#include <cstdlib>

#define QUEUE_NAME "/test_queue"
#define MAX_SIZE 1024
#define STOP_MSG "exit"

int main() {
    mqd_t mq;
    char buffer[MAX_SIZE];

    // 打开或创建消息队列
    mq = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT, 0644, nullptr);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    std::cout << "Message receiver started.\n";

    while (true) {
        memset(buffer, 0, MAX_SIZE);

        // 接收消息
        ssize_t bytes_read = mq_receive(mq, buffer, MAX_SIZE, nullptr);
        if (bytes_read == -1) {
            perror("mq_receive");
            break;
        }

        std::cout << "Received: " << buffer << std::endl;

        if (strcmp(buffer, STOP_MSG) == 0) {
            break;
        }
    }

    // 关闭并删除消息队列
    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    return 0;
}
