// tcp_server.cpp
#include <iostream> // 包含输入输出流库
#include <cstring> // 包含字符串处理库
#include <sys/types.h> // 包含基本系统数据类型
#include <sys/socket.h> // 包含套接字库
#include <netinet/in.h> // 包含Internet地址族库
#include <unistd.h> // 包含Unix标准库

#define PORT 8080 // 定义端口号
#define BUFFER_SIZE 1024 // 定义缓冲区大小

int main() {
    int server_fd, new_socket; // 定义服务器文件描述符和新套接字
    struct sockaddr_in address; // 定义地址结构
    int opt = 1; // 定义选项变量
    int addrlen = sizeof(address); // 获取地址结构大小
    char buffer[BUFFER_SIZE] = {0}; // 定义缓冲区并初始化

    // 创建 socket 文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed"); // 输出错误信息
        exit(EXIT_FAILURE); // 退出程序
    }

    // 附加 socket 到端口
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt"); // 输出错误信息
        exit(EXIT_FAILURE); // 退出程序
    }
    address.sin_family = AF_INET; // 设置地址族为IPv4
    address.sin_addr.s_addr = INADDR_ANY; // 设置IP地址为本地任意地址
    address.sin_port = htons(PORT); // 设置端口号

    // 绑定 socket 到端口
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed"); // 输出错误信息
        exit(EXIT_FAILURE); // 退出程序
    }

    // 监听端口
    if (listen(server_fd, 3) < 0) {
        perror("listen"); // 输出错误信息
        exit(EXIT_FAILURE); // 退出程序
    }

    // 接受客户端连接
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept"); // 输出错误信息
        exit(EXIT_FAILURE); // 退出程序
    }

    // 读取客户端发送的数据
    read(new_socket, buffer, BUFFER_SIZE); // 读取数据到缓冲区
    std::cout << "Message from client: " << buffer << std::endl; // 输出客户端消息

    // 发送响应给客户端
    const char *response = "Hello from server"; // 定义响应消息
    send(new_socket, response, strlen(response), 0); // 发送响应消息
    std::cout << "Response sent to client" << std::endl; // 输出发送消息提示

    // 关闭 socket
    close(new_socket); // 关闭新套接字
    close(server_fd); // 关闭服务器文件描述符

    return 0; // 返回0表示程序正常结束
}