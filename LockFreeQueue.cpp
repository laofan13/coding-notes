#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

template <typename T>
class LockFreeQueue
{
private:
    struct Node
    {
        T     data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;

public:
    LockFreeQueue() : head(nullptr), tail(nullptr) {}

    ~LockFreeQueue()
    {
        while (Node* node = head.load()) {
            head.store(node->next);
            delete node;
        }
    }

    bool Empty()
    {
        return head.load() == tail.load();
    }

    void Push(const T& value)
    {
        Node* newNode = new Node(value);
        newNode->next = nullptr;

        Node* curTail = tail.exchange(newNode);
        if (curTail) {
            curTail->next = newNode;
        } else {
            std::cout << "new Head" << std::endl;
            head.store(newNode, std::memory_order_release);
        }

        // Node* curTail = tail.load();
        // if(curTail == nullptr) {
        //     head.store(newNode);
        // }

        // while (curTail && !tail.compare_exchange_weak(curTail, newNode)) {
        //     // 尝试更新头指针直到成功或者队列为空
        //     curTail->next = newNode;
        // }
    }

    bool Pop(T& result)
    {
        Node* currHead = head.load(std::memory_order_acquire);    // 原子操作

        while (currHead && !head.compare_exchange_weak(currHead, currHead->next)) {
            // 尝试更新头指针直到成功或者队列为空
            std::cout << "尝试更新头指针直到成功或者队列为空"
                      << "\n";
        }

        if (currHead) {
            result = currHead->data;
            delete currHead;
            return true;
        }

        return false;
    }
};

// clang++ -o LockFreeQueue LockFreeQueue.cpp -std=c++17 -lpthread
int main()
{
    LockFreeQueue<int> queue;

    std::thread thread1([&queue]() {
        for (int j = 0; j < 10; j++) {
            queue.Push(j);
            std::cout << "Enqueue a element:" << j << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(50));    // 睡眠50毫秒
        }
        std::cout << std::endl;
    });

    std::thread thread2([&queue]() {
        int i   = 0;
        int num = 0;
        while (i < 2) {
            if (!queue.Pop(num)) {
                // std::cout << "faild to Dequeue  a element:" << num << "\n";
                continue;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));    // 睡眠50毫秒
            std::cout << "Dequeue a element:" << num << "\n";
            i++;
        }
    });

    if (thread1.joinable()) thread1.join();

    if (thread2.joinable()) thread2.join();

    return 0;
}