#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

template <typename T>
class LockFreeStack {
private:
	struct Node {
		T data;
		Node *next;

		Node(const T &value) : data(value), next(nullptr) {
		}
	};

	std::atomic<Node *> head;

public:
	LockFreeStack() : head(nullptr) {
	}

	~LockFreeStack() {
		while (Node *node = head.load()) {
			head.store(node->next);
			delete node;
		}
	}

	void Push(const T &value) {
		Node *currHead = head.load(); // 原子操作
		// new node
		Node *newNode = new Node(value);
		newNode->next = currHead;

		while (!head.compare_exchange_weak(currHead, newNode)) {
			// 尝试更新头指针直到成功或者队列为空
			newNode->next = currHead;
		}

		// while (currHead && !head.compare_exchange_weak(newNode->next, newNode)) {
		//     // 尝试更新头指针直到成功或者队列为空
		// }
	}

	bool Empty() {
		return head.load() == nullptr;
	}

	T &Top() {
		return head.load()->data;
	}

	bool Pop(T &result) {
		Node *currHead = head.load(); // 原子操作

		while (currHead && !head.compare_exchange_weak(currHead, currHead->next)) {
			// 尝试更新头指针直到成功或者队列为空
		}

		if (currHead) {
			result = currHead->data;
			delete currHead;
			return true;
		}

		return false;
	}
};

// clang++ -o LockFreeStack LockFreeStack.cpp -std=c++17 -lpthread
int main() {
	LockFreeStack<int> queue;

	std::thread thread1([&queue]() {
		for (int j = 0; j < 10; j++) {
			queue.Push(j);
			std::cout << "Push a element:" << j << "\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 睡眠50毫秒
		}
		std::cout << std::endl;
	});

	std::thread thread2([&queue]() {
		int i = 0;
		int num = 0;
		while (i < 10) {
			if (!queue.Pop(num)) {
				// std::cout << "faild to Pop  a element:" << num << "\n";
				continue;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 睡眠50毫秒
			std::cout << "pop a element:" << num << "\n";
			i++;
		}
	});

	if (thread1.joinable())
		thread1.join();

	if (thread2.joinable())
		thread2.join();

	return 0;
}