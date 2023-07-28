#include <iostream>
#include <unordered_map>
#include <vector>

struct Node {
  int key;
  int value;
  Node *left;
  Node *right;
  Node(int k, int v) : key(k), value(v) {
    left = nullptr;
    right = nullptr;
  }
};

struct BiList {
  Node *head;
  Node *tail;

  BiList() {
    head = new Node(0, 0);
    tail = head;
  }

  void insert_front(Node *node) {
    auto first = head->right;
    node->right = first;
    head->right = node;
    node->left = head;
    if (first) {
      first->left = node;
    }
    if (tail == head)
      tail = head->right;
  }

  std::pair<int, int> erase_end() {
    if (tail == head)
      return {-1, -1};
    Node *tmp = tail;
    tmp->left->right = nullptr;
    tail = tmp->left;
    int key = tmp->key, val = tmp->value;
    delete tmp;
    return {key, val};
  }

  void erase(Node *node) {
    if (node == tail)
      tail = node->left;
    auto left = node->left;
    auto right = node->right;
    left->right = right;
    if (right)
      right->left = left;
    delete node;
  }

  Node *first() { return head->right; }

  ~BiList() {
    Node *ptr = head;

    while (ptr) {
      Node *tmp = ptr->right;

      delete ptr;
      ptr = tmp;
    }
  }
};

class LRUcache {
private:
  int cap;
  BiList *lst;
  std::unordered_map<int, Node *> mp;

public:
  LRUcache(int k) : cap(k) { lst = new BiList(); }
  void set(int key, int value) {
    if (mp.find(key) == mp.end()) {
      if (mp.size() == cap) { // evict
        auto p = lst->erase_end();
        int rm_key = p.first;
        mp.erase(rm_key);
      }
    } else {
      auto node = mp[key];
      lst->erase(node);
    }
    lst->insert_front(new Node(key, value));
    mp[key] = lst->first();
  }

  int get(int key) {
    if (mp.find(key) == mp.end())
      return -1;
    auto node = mp[key];
    int value = node->value;
    lst->erase(node);
    lst->insert_front(new Node(key, value));
    mp[key] = lst->first();
    return value;
  }
  ~LRUcache() { delete lst; }
};

int main() {
  int n, k;
  std::cin >> n >> k;
  LRUcache cache(k);
  std::vector<int> res;
  for (int i = 0; i < n; ++i) {
    int opt;
    std::cin >> opt;
    if (opt == 1) {
      int x, y;
      std::cin >> x >> y;
      cache.set(x, y);
    } else {
      int x;
      std::cin >> x;
      res.push_back(cache.get(x));
    }
  }
  for (int num : res)
    std::cout << num << " ";
  return 0;
}
