#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

class MessageQueue {
public:
    void send(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(message);
        cond_var_.notify_one(); // 通知消费者
    }

    std::string receive() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [this]() { return !queue_.empty(); }); // 等待消息
        std::string message = queue_.front();
        queue_.pop();
        return message;
    }

private:
    std::queue<std::string> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};

void producer(MessageQueue& mq) {
    std::string messages[] = {"Hello", "World", "Thread", "MessageQueue", "exit"};
    for (const auto& msg : messages) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 模拟生产过程
        std::cout << "[Producer] Sending: " << msg << std::endl;
        mq.send(msg);
    }
}

void consumer(MessageQueue& mq) {
    while (true) {
        std::string msg = mq.receive();
        std::cout << "[Consumer] Received: " << msg << std::endl;
        if (msg == "exit") break; // 退出信号
    }
}

int main() {
    MessageQueue mq;

    std::thread producer_thread(producer, std::ref(mq));
    std::thread consumer_thread(consumer, std::ref(mq));

    producer_thread.join();
    consumer_thread.join();

    return 0;
}
