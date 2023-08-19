// https://zhuanlan.zhihu.com/p/642416997
// Sequential consistency

#include <atomic>
#include <thread>
#include <iostream>

std::atomic<int> x(0), y(0), z(0);
std::atomic<bool> go(false);

void write_x() {
    x.store(1, std::memory_order_seq_cst); //L1
    std::cout << "write_x done" << std::endl;
    go.store(true, std::memory_order_seq_cst); //L2
}

void write_y() {
    while (!go.load(std::memory_order_seq_cst)) { //L3
        std::this_thread::yield();
    }
    y.store(1, std::memory_order_seq_cst); //L4
    std::cout << "write_y done" << std::endl;
}

void read_x_then_y() {
    while (!go.load(std::memory_order_seq_cst)) { //L5
        std::this_thread::yield();
    }
    int r1 = x.load(std::memory_order_seq_cst); //L6
    int r2 = y.load(std::memory_order_seq_cst); //L7
    if (r1 == 1 && r2 == 1) { 
        z.store(1, std::memory_order_seq_cst); //L8
    }
    std::cout << "read_x_then_y done" << std::endl;
}

void read_y_then_x() {
    while (!go.load(std::memory_order_seq_cst)) { //L9
        std::this_thread::yield();
    }
    int r1 = y.load(std::memory_order_seq_cst); //L10
    int r2 = x.load(std::memory_order_seq_cst); //L11
    if (r1 == 1 && r2 == 0) {
        z.store(2, std::memory_order_seq_cst); //L12
    }
    std::cout << "read_y_then_x done" << std::endl;
}



int main() {
    std::thread t1(write_x);
    std::thread t2(write_y);
    std::thread t3(read_x_then_y);
    std::thread t4(read_y_then_x);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    std::cout << "z = " << z.load(std::memory_order_seq_cst) << std::endl;
    return 0;
}