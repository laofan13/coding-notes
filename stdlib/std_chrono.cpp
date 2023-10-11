#include <chrono>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>

void test1() {
	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	// std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

	// std::chrono::system_clock::time_point end_time = std::chrono::system_clock::now();
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t tt = std::chrono::system_clock::to_time_t(now);
	std::tm *ptm = std::localtime(&tt);
	std::cout << "Current time is: " << std::put_time(ptm, "%c") << std::endl;

	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	std::chrono::system_clock::duration elapsed = end - start;

	auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
}

void print_time() {
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(localtime(&in_time_t), "%Y-%m-%d %X");
  std::cout << "now is: " << ss.str() << std::endl;
}

void test2() {
	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm *now_tm = std::localtime(&t);

	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);

	std::cout << "Current time: " << buffer << std::endl;
}

void test3() {
	std::chrono::seconds sec(1);
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(sec);
}

void test_time_point() {
	// 获取当前的时间点
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

	// 转换为时间戳并打印
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	std::cout << "Current time: " << std::ctime(&now_c) << std::endl;
}

void test_duration() {
	std::chrono::duration<int> twenty_seconds(20);
	std::chrono::duration<double, std::ratio<60>> half_a_minute(0.5);
	std::chrono::duration<long, std::ratio<1, 1000>> one_millisecond(1);
}

int main() {
	test1();
	test2();
	test3();
	test_time_point();
	test_duration();
	return 0;
}