// https://zhuanlan.zhihu.com/p/642416997
// Relax
#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> x(0);

void increase() {
    for (int i = 0; i < 100000; ++i) {
        x.fetch_add(1, std::memory_order_relaxed);
    }
}

// std::atomic<int> x(0), y(0);

// void thread1() {
//     x.store(1, std::memory_order_relaxed); //L1
//     y.store(2, std::memory_order_relaxed); //L2
// }

// void thread2() {
//     while (y.load(std::memory_order_relaxed) != 2); //L3
//     std::cout << "x = " << x.load(std::memory_order_relaxed) << std::endl; //L4
// }

int main() {
    std::thread t1(increase);
    std::thread t2(increase);
    t1.join();
    t2.join();
    std::cout << "x = " << x << std::endl;
    return 0;
}