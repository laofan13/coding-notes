#include <atomic>
#include <future>
#include <iostream>
#include <thread>

double concurrent_worker(int min, int max) {
	double sum = 0;
	for (int i = min; i <= max; i++) {
		sum += sqrt(i);
	}
	return sum;
}

double test_packaged_task(int min, int max) {
	std::vector<std::future<double>> results; // ①

	unsigned concurrent_count = std::thread::hardware_concurrency();
	min = 0;
	for (int i = 0; i < concurrent_count; i++) {                      // ②
		std::packaged_task<double(int, int)> task(concurrent_worker); // ③
		results.push_back(task.get_future());                         // ④

		int range = max / concurrent_count * (i + 1);
		std::thread t(std::move(task), min, range); // ⑤
		t.detach();

		min = range + 1;
	}

	std::cout << "threads create finish" << std::endl;
	double sum = 0;
	for (auto &r : results) {
		sum += r.get(); // ⑥
	}
	return sum;
}

void test_async() {
	double result = 0;
	std::cout << "Async task with lambda triggered, thread: " << std::this_thread::get_id() << std::endl;
	auto f2 = std::async(std::launch::async, [&result]() {
		std::cout << "Lambda task in thread: " << std::this_thread::get_id() << std::endl;
		for (int i = 0; i <= 100; i++) {
			result += sqrt(i);
		}
	});
	f2.wait();
	std::cout << "Async task with lambda finish, result: " << result << std::endl << std::endl;
}

void test_promise(int min, int max, std::promise<double>* result) { // ①
  std::vector<std::future<double>> std::results;

  unsigned concurrent_count = std::thread::hardware_concurrency();
  min = 0;
  for (int i = 0; i < concurrent_count; i++) {
    std::packaged_task<double(int, int)> task(concurrent_worker);
    results.push_back(task.get_future()); 

    int range = max / concurrent_count * (i + 1);
    std::thread t(std::move(task), min, range);
    t.detach();

    min = range + 1;
  }

  std::cout << "threads create finish" << std::endl;
  double sum = 0;
  for (auto& r : results) {
    sum += r.get();
  }
  result->set_value(sum); // ②
  std::cout << "concurrent_task finish" << std::endl;
}

int main() {

	return 0;
}