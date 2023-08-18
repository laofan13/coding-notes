
#include <array>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <mutex>
#include <vector>

// Class template for Skip list
template <typename KeyType, typename ValueType>
class SkipList {
public:
	struct Node {
		KeyType key;
		ValueType value;
		int level;
		std::vector<Node *> forward;

		Node(int l) : level(l), forward(l + 1, 0) {
		}

		Node(const KeyType k, const ValueType v, int l) : key(k), value(v), level(l), forward(l + 1, 0) {
		}
	};

	using NodePtr = Node *;

	SkipList(int level)
	    : max_level(level), skip_list_level(0), element_count(0), header(new Node(level)) {

	                                                              };

	~SkipList() {
		NodePtr p = header;
		NodePtr q = nullptr;
		while (p) {
			q = p->forward[0];
			free(p);
			p = q;
		}
	}

	int RandomLevel() {
		int level = 1;
		while (rand() % 2) {
			level++;
		}
		return (level < max_level) ? level : max_level;
	}

	bool Insert(const KeyType key, const ValueType value) {
		NodePtr update[max_level + 1];
		auto cur = this->header;
		// start form highest level of skip list
		for (int i = skip_list_level; i >= 0; i--) {
			while (cur->forward[i] && cur->forward[i]->key < key) {
				cur = cur->forward[i];
			}
			update[i] = cur;
		}

		// reached level 0 and forward pointer to right node, which is desired to insert key.
		cur = cur->forward[0];

		// 如果key已经存在
		if (cur && cur->key == key) {
			cur->value = value;
			std::cout << "Key alreadly exsit:" << key << ", value:" << value << std::endl;
			return false;
		}

		// 随机生成新结点的层数
		int level = RandomLevel();
		// 为了节省空间，采用比当前最大层数加1的策略
		if (level > skip_list_level) {
			level = ++skip_list_level;
			update[level] = header;
		}

		NodePtr new_node = new Node(key, value, level);

		// 调整forward指针
		for (int i = level; i >= 0; --i) {
			cur = update[i];
			new_node->forward[i] = cur->forward[i];
			cur->forward[i] = new_node;
		}
		std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
		++element_count;
		return true;
	}

	bool Search(const KeyType &key, ValueType &value) {
		auto cur = this->header;
		// start form highest level of skip list
		for (int i = skip_list_level; i >= 0; i--) {
			while (cur->forward[i] && cur->forward[i]->key < key) {
				cur = cur->forward[i];
			}
		}

		cur = cur->forward[0];
		if (cur == nullptr || cur->key != key) {
			std::cout << "Not Found Key:" << key << std::endl;
			return false;
		}

		value = cur->value;
		std::cout << "Found key: " << key << ", value: " << value << std::endl;
		return true;
	}

	bool Delete(const KeyType &key) {
		NodePtr update[max_level + 1];
		auto cur = this->header;
		// start form highest level of skip list
		for (int i = skip_list_level; i >= 0; i--) {
			while (cur->forward[i] && cur->forward[i]->key < key) {
				cur = cur->forward[i];
			}
			update[i] = cur;
		}

		// reached level 0 and forward pointer to right node, which is desired to insert key.
		cur = cur->forward[0];
		if (cur == nullptr || cur->key != key) {
			std::cout << "Faild to delete Key: " << key << std::endl;
			return false;
		}

		// 调整指针
		for (int i = 0; i <= skip_list_level; ++i) {
			if (update[i]->forward[i] != cur)
				break;
			update[i]->forward[i] = cur->forward[i];
		}
		// 删除结点
		delete cur;

		// 更新level的值，有可能会变化，造成空间的浪费
		while (skip_list_level > 0 && header->forward[skip_list_level] == nullptr) {
			--skip_list_level;
		}

		// 更新链表元素数目
		--element_count;
		std::cout << "Successfully deleted key " << key << std::endl;
		return true;
	}

	void display_list() {
		std::cout << "\n*****Skip List*****"
		          << "\n";
		for (int i = 0; i <= skip_list_level; i++) {
			auto cur = header->forward[i];
			std::cout << "Level " << i << ": ";
			while (cur != NULL) {
				std::cout << cur->key << ":" << cur->value << ";";
				cur = cur->forward[i];
			}
			std::cout << std::endl;
		}
	}

	int size() {
		return element_count;
	}

private:
	// Maximum level of the skip list
	int max_level;

	// cur level of skip list
	int skip_list_level;

	// skiplist cur element count
	int element_count;

	// pointer to header node
	NodePtr header;
};

// clang++ -o SkipList SkipList.cpp -std=c++17
// 最大层数
static constexpr int MAX_LEVEL = 16;

int main(int argc, char **argv) {
	std::cout << "test is starting ....."
	          << "\n";

	SkipList<int, int> skip_list(MAX_LEVEL);

	// 测试插入
	for (int i = 0; i < 100; ++i) {
		skip_list.Insert(i, i * 2);
		// cout << list.GetCurrentLevel() << endl;
	}
	std::cout << "The number of elements in SkipList is :" << skip_list.size() << "\n";
	assert(skip_list.size() == 100);
	skip_list.display_list();

	// 测试查找
	for (int i = 0; i < 100; ++i) {
		int value;
		if (skip_list.Search(i, value)) {
			assert(value == i * 2);
		}
	}

	// 测试删除
	for (int i = 0; i < 100; ++i) {
		if (i % 2 == 0) {
			assert(skip_list.Delete(i));
		}
	}
	assert(skip_list.size() == 50);
	skip_list.display_list();

	// //测试删除
	// for(int i = 100; i < 200; ++i){
	//     int value;
	//     assert(!skip_list.Search(i, value));
	// }

	// //测试删除
	// for(int i = 0; i < 100; ++i){
	//     assert(skip_list.Delete(i));
	// }

	return 0;
}