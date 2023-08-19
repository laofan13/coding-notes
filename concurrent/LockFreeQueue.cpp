#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

template <typename T>
class LockFreeQueue {
private:
    int capacity;
	std::vector<T> data;

	std::atomic<int> front_ {0};
	std::atomic<int> back_ {0};
	std::atomic<int> write_ {0};

public:
	explicit LockFreeQueue(int cap) : capacity(cap), data(capacity) {
	}

	~LockFreeQueue() = default;

    bool Empty() {
        return front_.load(std::memory_order_relaxed) == back_.load(std::memory_order_acquire);
    }

    size_t Size() {
        return back_.load(std::memory_order_acquire) - front_.load(std::memory_order_relaxed);
    } 

	bool Push(const T &value) {
        int write = 0;
        do {
            write = write_.load(std::memory_order_relaxed);
            if((write + 1) % capacity == front_.load(std::memory_order_acquire)){
                return false;
            }
        }while(!write_.compare_exchange_strong(write, (write + 1) % capacity, std::memory_order_relaxed)); //L3
        data[write] = value;

        int back = 0;
        do {
            back = write;
        } while(!back_.compare_exchange_strong(back, (back + 1) % capacity , std::memory_order_release));
    
        return true;
	}

	bool Pop(T &result) {
		int front = 0;
        do {
            front = front_.load(std::memory_order_relaxed); 
            if(front == back_.load(std::memory_order_acquire)){
                return false;
            }
            result = data[front]; 
        }while(!front_.compare_exchange_strong(front, (front + 1) % capacity, std::memory_order_release)); //L9
        std::cout << "front: "<< front_ << " back: "<<back_ << std::endl; 
        return true;
	}
};

// clang++ -o LockFreeQueue LockFreeQueue.cpp -std=c++17 -lpthread
int main() {
	LockFreeQueue<int> queue(50);
    int n = 100;
	std::thread thread1([n, &queue]() {
        int i = 0;
        while(i < n) {
            if(!queue.Push(i * 2)) {
                continue;
            }
            std::cout << "Enqueue a element:" << i << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 睡眠50毫秒
            i++;
        }
		std::cout << std::endl;
	});

	std::thread thread2([n, &queue]() {
		int i = 0;
		int val = 0;
		while (i < n) {
			if (!queue.Pop(val)) {
                // std::cout << "Dequeue faild:" << val << "\n";
				continue;
			}
            assert(val == 2 * i);
			std::this_thread::sleep_for(std::chrono::milliseconds(10)); // 睡眠50毫秒
			std::cout << "Dequeue a element:" << val << "\n";
			i++;
		}
	});

    assert(queue.Empty());

	if (thread1.joinable())
		thread1.join();

	if (thread2.joinable())
		thread2.join();

	return 0;
}