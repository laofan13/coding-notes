
#include <iostream>
#include <chrono>
#include <functional>
#include <map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

// clang++ -o Timer Timer.cpp -std=c++17 -lpthread

class Timer {
public:
    struct TimerTask {
        std::chrono::time_point<std::chrono::high_resolution_clock> time_point;
        std::function<void()> task;

        bool operator<(const TimerTask &other) const {
            return time_point > other.time_point;
        }
    };

    enum class State { 
        INIT, 
        RUNNING,
        STOP
    };

    Timer():running_(true)
    {
        repeated_task_id_.store(0);    
    }


    void Run() {
        std::thread([this]() { 
            while (running_) {
                std::unique_lock<std::mutex> lock(mutex_);
                cond_.wait(lock, [this](){
                    return !queue_.empty();
                });

                auto s = queue_.top();
                auto duration = s.time_point - std::chrono::high_resolution_clock::now();
                auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
                if (milliseconds.count() > 0) {
                    cond_.wait_for(lock, duration);
                    continue;
                } 
                queue_.pop();
                lock.unlock();

                // run task
                s.task();
            }
        }).detach();
    }


    void Stop() {
        running_ = false;
        cond_.notify_all();
    }

    template <typename F, typename... Args>
    void AddTaskWithTimePoint(const std::chrono::time_point<std::chrono::high_resolution_clock>& time_point, 
            F&& f,
            Args&&... args) {
        TimerTask t;
        t.time_point = time_point;
        t.task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

        {
            std::unique_lock<std::mutex> lock(mutex_);
            queue_.push(t);
        }
        cond_.notify_all();
    }

    template <typename R, typename P, typename F, typename... Args>
    void AddTaskAfterDuration(const std::chrono::duration<R, P>& time, F&& f, Args&&... args) {
        TimerTask t;
        t.time_point = std::chrono::high_resolution_clock::now() + time;
        t.task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

        {
            std::unique_lock<std::mutex> lock(mutex_);
            queue_.push(t);
        }
        cond_.notify_all();
    }

    template <typename R, typename P, typename F, typename... Args>
    int AddRepeatedTask(int repeat_num, const std::chrono::duration<R, P>& time, F&& f, Args&&... args) {
        int id = NextTaskId();
        repeated_id_state_map_.emplace(id, State::RUNNING);

        auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
        AddRepeatedTaskLocal(repeat_num - 1, time, id, std::move(task));
        return id;
    }

    int NextTaskId() { return repeated_task_id_++; }

    template <typename R, typename P, typename F>
    void AddRepeatedTaskLocal(int repeat_num, const std::chrono::duration<R, P>& time, int id, F&& f) {
        if (repeated_id_state_map_.find(id) == repeated_id_state_map_.end()) {
            return;
        }
        TimerTask t;
        t.time_point = std::chrono::high_resolution_clock::now() + time;
        auto temp_task = std::move(f);
        t.task = [this, &temp_task, repeat_num, time, id]() {
            temp_task();
            if (repeated_id_state_map_.find(id) == repeated_id_state_map_.end() || repeat_num == 0) {
                return;
            }
            AddRepeatedTaskLocal(repeat_num - 1, time, id, std::move(temp_task));
        };

        {
            std::unique_lock<std::mutex> lock(mutex_);
            queue_.push(t);
        }
        cond_.notify_all();
    }


    void CancelRepeatedTask(int id) { repeated_id_state_map_.erase(id); }

private:
    std::priority_queue<TimerTask> queue_;

    bool running_ = false;
    std::mutex mutex_;
    std::condition_variable cond_;

    std::atomic<int> repeated_task_id_;
    std::map<int, State> repeated_id_state_map_;
};

int main() {
    Timer timer;
    timer.Run();
    for (int i = 5; i < 15; ++i) {
        timer.AddTaskAfterDuration(std::chrono::seconds(i + 1), [i]() { 
            std::cout << "this is " << i << std::endl; 
        });

        timer.AddTaskWithTimePoint(std::chrono::high_resolution_clock::now() + std::chrono::seconds(1),
            [](int i) { 
                std::cout << "this is " << i << " at " << std::endl; 
        }, i);
    }

    int id = timer.AddRepeatedTask(10, std::chrono::seconds(1), []() { 
        std::cout << "repeate task " << std::endl; 
    });
    std::this_thread::sleep_for(std::chrono::seconds(4));
    timer.CancelRepeatedTask(id);
    std::this_thread::sleep_for(std::chrono::seconds(30));
    timer.Stop();
    return 0;

    
}

mai