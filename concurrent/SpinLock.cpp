#include <atomic>
#include <thread>

class SpinLock {
private:
    std::atomic<bool> flag_ = {false};
public:
    void Lock() {
        while (flag_.exchange(true, std::memory_order_acquire)); 
    }

    void Unlock() {
        flag_.store(false, std::memory_order_release); 
    }
};