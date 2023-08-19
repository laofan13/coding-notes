// Acquire-Release模型

#include <atomic>
#include <thread>
#include <iostream>

std::atomic<bool> flag(false);
int data = 0;

void producer() {
    data = 100; //L1
    flag.store(true, std::memory_order_release); //L2
}

void consumer() {
    while (!flag.load(std::memory_order_acquire)); //L3
    std::cout << "data = " << data << std::endl; //L4
}

int main() {
    std::thread t1(producer);
    std::thread t2(consumer);
    t1.join();
    t2.join();
    return 0;
}