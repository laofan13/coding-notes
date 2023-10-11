#include <atomic>
#include <thread>

class SpinLock {
private:
    std::atomic_flag flag_;
public:
    void Lock() {
        while (flag_.test_and_set(std::memory_order_acquire)); 
    }

    void Unlock() {
        flag_.clear(std::memory_order_release); 
    }
};