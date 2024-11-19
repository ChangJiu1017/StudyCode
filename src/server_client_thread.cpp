#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <atomic>
#include <semaphore.h>
#include <mutex>
#include <cstring>
#include <condition_variable>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

constexpr int SHARED_MEMORY_SIZE = 1024; // 共享内存大小
constexpr char SHM_NAME[] = "/shm_example";
constexpr int THREAD_POOL_SIZE = 4;

// 自旋锁类
class SpinLock {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire));
    }
    void unlock() {
        flag.clear(std::memory_order_release);
    }
};

// 消息结构
struct Message {
    int client_id;
    char data[256];
};

// 服务端线程池
class ThreadPool {
    std::vector<std::thread> workers;
    std::queue<Message> taskQueue;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;

public:
    ThreadPool(size_t threads) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    Message task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this]() { return stop || !taskQueue.empty(); });
                        if (stop && taskQueue.empty())
                            return;
                        task = taskQueue.front();
                        taskQueue.pop();
                    }
                    processTask(task);
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : workers)
            worker.join();
    }

    void addTask(const Message& task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            taskQueue.push(task);
        }
        condition.notify_one();
    }

private:
    void processTask(const Message& task) {
        std::cout << "Processing client " << task.client_id << ": " << task.data << std::endl;
        // 模拟任务处理时间
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};

// 服务端类
class Server {
    ThreadPool threadPool;
    SpinLock spinLock;
    sem_t semaphore;

    int shm_fd;
    void* shared_memory;

public:
    Server() : threadPool(THREAD_POOL_SIZE) {
        // 创建共享内存
        shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SHARED_MEMORY_SIZE);
        shared_memory = mmap(nullptr, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

        // 初始化信号量
        sem_init(&semaphore, 0, 0);
    }

    ~Server() {
        munmap(shared_memory, SHARED_MEMORY_SIZE);
        shm_unlink(SHM_NAME);
        sem_destroy(&semaphore);
    }

    void start() {
        std::thread([this]() {
            while (true) {
                // 等待信号量通知
                sem_wait(&semaphore);
                
                // 使用自旋锁读取共享内存数据
                spinLock.lock();
                Message message;
                std::memcpy(&message, shared_memory, sizeof(Message));
                spinLock.unlock();

                // 将任务交给线程池
                threadPool.addTask(message);
            }
        }).detach();
    }

    void notify() {
        sem_post(&semaphore);
    }
};

// 客户端类
class Client {
    int shm_fd;
    void* shared_memory;

public:
    Client() {
        // 打开共享内存
        shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
        shared_memory = mmap(nullptr, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    }

    ~Client() {
        munmap(shared_memory, SHARED_MEMORY_SIZE);
    }

    void sendMessage(int client_id, const std::string& message) {
        Message msg;
        msg.client_id = client_id;
        std::strncpy(msg.data, message.c_str(), sizeof(msg.data));

        // 写入共享内存
        SpinLock spinLock;
        spinLock.lock();
        std::memcpy(shared_memory, &msg, sizeof(Message));
        spinLock.unlock();
    }
};

// 测试代码
int main() {
    Server server;
    server.start();

    Client client1, client2;

    // 客户端 1 发送消息
    client1.sendMessage(1, "Hello from client 1");
    server.notify();

    // 客户端 2 发送消息
    client2.sendMessage(2, "Hello from client 2");
    server.notify();

    // 模拟运行一段时间
    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
