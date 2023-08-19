#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
	ThreadPool() = default;

	ThreadPool(const ThreadPool &) = delete;

	ThreadPool(ThreadPool &&) = delete;

	ThreadPool &operator=(const ThreadPool &) = delete;

	ThreadPool &operator=(ThreadPool &&) = delete;

	ThreadPool(size_t n_thread = 4) : stop(false) {
		assert(n_thread > 0);

		for (size_t i = 0; i < n_thread; i++) {
			workers.emplace_back([this] {
				while (1) {
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(this->m_mutex);
						m_cv.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
						if (this->stop && this->tasks.empty())
							break;
						auto task = std::move(tasks.front());
						tasks.pop();
					}
					task();
				}
			});
		}
	}

	// return : std::future<decltype(f(args...))>
	// return(C++14): std::future<typename std::result_of<F(Args...)>::type>
	// return(C++17): std::future<typename std::invoke_result_t<F,
	// Args...)>::type>
	template <class F, class... Args>
	auto enqueue(F &&f, Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type> {
		using RetType = typename std::result_of<F(Args...)>::type;
		// decltype(f(args...)); // typename std::result_of<F(Args...)>::type
		auto task =
		    std::make_shared<std::packaged_task<RetType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

		std::future<RetType> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(m_mutex);

			// don't allow enqueueing after stopping the pool
			if (stop)
				throw std::runtime_error("enqueue on stopped ThreadPool");

			tasks.emplace([&task]() { (*task)(); });
		}
		m_cv.notify_one();
		return res;
	}

	~ThreadPool() {
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			stop = true;
		}
		m_cv.notify_all();

		for (int i = 0; i < workers.size(); ++i) {
			if (workers[i].joinable())
				workers[i].join();
		}
	}

private:
	using Task = std::function<void()>;

	std::queue<Task> tasks;
	std::vector<std::thread> workers;

	std::mutex m_mutex;
	std::condition_variable m_cv;

	// std::atomic<bool> stop{ false };
	bool stop {false};
};
