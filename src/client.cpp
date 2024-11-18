#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // 创建套接字
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        return -1;
    }

    // 配置服务器地址
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 将地址转换为二进制
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address or Address not supported");
        close(sock);
        return -1;
    }

    // 连接到服务端
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return -1;
    }

    std::cout << "Connected to the server.\n";

    // 与服务端通信
    while (true) {
        std::cout << "Enter message: ";
        std::string message;
        std::getline(std::cin, message);
        if (message == "exit") {
            std::cout << "Exiting...\n";
            break;
        }
        send(sock, message.c_str(), message.length(), 0);

        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(sock, buffer, BUFFER_SIZE);
        if (valread <= 0) {
            std::cout << "Server disconnected.\n";
            break;
        }
        std::cout << "Server echoed: " << buffer << "\n";
    }

    close(sock);
    return 0;
}
