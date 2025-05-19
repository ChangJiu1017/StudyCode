
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstring>
#include "linux_fstream.h"


// 模拟数据包的结构
struct Packet {
    std::string data;
    std::string sourceIP;
    std::string destIP;
    int sourcePort;
    int destPort;
};

// 模拟应用层协议（如 HTTP）
class ApplicationLayer {
public:
    static Packet createRequest(const std::string& data, const std::string& sourceIP, const std::string& destIP, int sourcePort, int destPort) {
        Packet packet;
        packet.data = "HTTP REQUEST: " + data;
        packet.sourceIP = sourceIP;
        packet.destIP = destIP;
        packet.sourcePort = sourcePort;
        packet.destPort = destPort;
        return packet;
    }

    static void handleResponse(const Packet& packet) {
        std::cout << "Application Layer: Received response: " << packet.data << std::endl;
    }
};

// 模拟传输层协议（如 TCP）
class TransportLayer {
public:
    static Packet encapsulateInTCP(Packet packet) {
        std::cout << "Transport Layer: Encapsulating data with TCP header." << std::endl;
        // 添加TCP头
        packet.data = "TCP HEADER + " + packet.data;
        return packet;
    }

    static Packet decapsulateFromTCP(Packet packet) {
        std::cout << "Transport Layer: Decapsulating TCP header." << std::endl;
        // 移除TCP头
        if (packet.data.find("TCP HEADER + ") == 0) {
            packet.data = packet.data.substr(13);
        }
        return packet;
    }
};

// 模拟网络层协议（如 IP）
class NetworkLayer {
public:
    static Packet encapsulateInIP(Packet packet) {
        std::cout << "Network Layer: Encapsulating data with IP header." << std::endl;
        // 添加IP头
        packet.data = "IP HEADER + " + packet.data;
        return packet;
    }

    static Packet decapsulateFromIP(Packet packet) {
        std::cout << "Network Layer: Decapsulating IP header." << std::endl;
        // 移除IP头
        if (packet.data.find("IP HEADER + ") == 0) {
            packet.data = packet.data.substr(11);
        }
        return packet;
    }
};

// 模拟数据链路层（如 Ethernet）
class DataLinkLayer {
public:
    static void sendFrame(Packet packet) {
        std::cout << "Data Link Layer: Sending frame to " << packet.destIP << " from " << packet.sourceIP << std::endl;
        std::cout << "Data Link Layer: Frame data: " << packet.data << std::endl;
    }

    static Packet receiveFrame(Packet packet) {
        std::cout << "Data Link Layer: Receiving frame." << std::endl;
        return packet;
    }
};

// 模拟整个发送过程
void sendPacket(const std::string& data, const std::string& sourceIP, const std::string& destIP, int sourcePort, int destPort) {
    std::cout << "---- Packet Sending ----" << std::endl;

    // 应用层
    Packet packet = ApplicationLayer::createRequest(data, sourceIP, destIP, sourcePort, destPort);

    // 传输层 (TCP)
    packet = TransportLayer::encapsulateInTCP(packet);

    // 网络层 (IP)
    packet = NetworkLayer::encapsulateInIP(packet);

    // 数据链路层 (Ethernet)
    DataLinkLayer::sendFrame(packet);

    std::cout << "---- Packet Sent ----\n" << std::endl;
}

// 模拟整个接收过程
void receivePacket(Packet packet) {
    std::cout << "---- Packet Receiving ----" << std::endl;

    // 数据链路层 (Ethernet)
    packet = DataLinkLayer::receiveFrame(packet);

    // 网络层 (IP)
    packet = NetworkLayer::decapsulateFromIP(packet);

    // 传输层 (TCP)
    packet = TransportLayer::decapsulateFromTCP(packet);

    // 应用层
    ApplicationLayer::handleResponse(packet);

    std::cout << "---- Packet Received ----\n" << std::endl;
}

// int main() {
//     std::string data = "GET /index.html";
//     std::string sourceIP = "192.168.1.2";
//     std::string destIP = "192.168.1.1";
//     int sourcePort = 8080;
//     int destPort = 80;

//     // 模拟发送数据包
//     sendPacket(data, sourceIP, destIP, sourcePort, destPort);

//     // 模拟接收数据包
//     Packet receivedPacket = ApplicationLayer::createRequest("HTTP RESPONSE: 200 OK", destIP, sourceIP, destPort, sourcePort);
//     receivePacket(receivedPacket);

//     return 0;
// }




int main()
{
    std::cout << "----     linux_fstream() ----" << std::endl;
    linux_fstream();
    
    return 0;
}
