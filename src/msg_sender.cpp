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
    mq = mq_open(QUEUE_NAME, O_WRONLY | O_CREAT, 0644, nullptr);
    if (mq == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    std::cout << "Message sender started. Type '" << STOP_MSG << "' to stop.\n";

    while (true) {
        std::cout << "Enter message: ";
        std::cin.getline(buffer, MAX_SIZE);

        // 发送消息
        if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1) {
            perror("mq_send");
            break;
        }

        if (strcmp(buffer, STOP_MSG) == 0) {
            break;
        }
    }

    // 关闭消息队列
    mq_close(mq);
    return 0;
}
