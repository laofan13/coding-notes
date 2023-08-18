#include <fstream>
#include <ifstream>
#include <iostream>
#include <ofstream>

void test1() {
	std::ofstream fs1("123.txt", std::ios::out);               // （创建文件）写入内容
	std::ifstream fs2("123.txt", std::ios::in);                // 读取内容
	std::fstream fs3("123.txt", std::ios::in | std::ios::out); // 以读写方式打开文件

	std::ifstream f("d:\\12.txt", std::ios::nocreate);
	// 默认ios::in 的方式打开文件，文件不存在时操作失败
	std::fstream f("d:\\12.dat", std::ios::in | std::ios::out | std::ios::binary);
	// 以读、写方式打开二进制文件
}

int main() {
	test1();
	return 0;
}