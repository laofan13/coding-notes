#include <algorithm>
#include <cmath>
#include <iostream>
#include <cassert>

struct Node
{
    int   key;
    int   val;
    int   height;
    Node *left, *right;

    Node() : key(0), val(0), height(0), left(nullptr), right(nullptr) {}

    Node(int k, int v, int h = 0) : key(k), val(v), height(h), left(nullptr), right(nullptr) {}
};

static Node* leftRotate(Node* root)
{
    Node* right      = root->right;
    Node* right_left = right->left;
    right->left      = root;
    root->right      = right_left;

    root->height  = std::max(root->left ? root->left->height : 0, 
        root->right ? root->right->height : 0) + 1;
    right->height = std::max(right->left ? right->left->height : 0,
        right->left ? right->left->height : 0) + 1;

    return right;
}

static Node* rightRotate(Node* root)
{
    Node* left       = root->left;
    Node* left_right = left->right;
    left->right      = root;
    root->left       = left_right;

    root->height  = std::max(root->left ? root->left->height : 0, 
        root->right ? root->right->height : 0) + 1;
    left->height = std::max(left->left ? left->left->height : 0,
        left->left ? left->left->height : 0) + 1;

    return left;
}

static Node* insert(Node* root, int key, int val)
{
    if (root == nullptr) return new Node(key, val, 1);

    if (key < root->key)
        root->left = insert(root->left, key, val);
    else
        root->right = insert(root->right, key, val);

    // adjust height
    int l_h   = root->left ? root->left->height : 0;
    int r_h   = root->right ? root->right->height : 0;
    root->height = std::max(l_h, r_h) + 1;

    // adjust balence
    int heightDiff = std::abs(l_h - r_h);
    if (heightDiff > 1) {
        if (l_h > r_h) {
            root = rightRotate(root);
        } else {
            root = leftRotate(root);
        }
    }
    return root;
}

static Node* search(Node* root, int key)
{
    if (root == nullptr) return nullptr;

    if (key < root->key) {
        return search(root->left, key);
    } else if (key > root->key) {
        return search(root->right, key);
    }
    return root;
}

static Node* mostLeftNode(Node* root)
{
    if (root->left == nullptr) {
        return root;
    }
    return mostLeftNode(root->left);
}

static Node* del(Node* root, int key)
{
    if (root == nullptr) return nullptr;

    if (key < root->key) {
        root->left = del(root->left, key);
    } else if (key > root->key) {
        root->right = del(root->right, key);
    } else {
        if (!root->right || !root->left) {
            auto temp = !root->right ? root->left : root->right;
            delete root;
            return temp;
        }

        // Node to be deleted have both left and right subtrees
        auto* temp  = mostLeftNode(root->right);
        root->key   = temp->key;
        root->val   = temp->val;
        root->right = del(root->right, temp->key);
    }

    int l_h   = root->left ? root->left->height : 0;
    int r_h   = root->right ? root->right->height : 0;
    root->height = std::max(l_h, r_h) + 1;

    // adjust balence
    int heightDiff = std::abs(l_h - r_h);
    if (heightDiff > 1) {
        if (l_h > r_h) {
            root = rightRotate(root);
        } else {
            root = leftRotate(root);
        }
    }

    return root;
}

static bool validateAvLTree(Node* root) {
    if(root->left && !validateAvLTree(root->left))
        return false;
    if(root->right && !validateAvLTree(root->right))
        return false;
    int l_h   = root->left ? root->left->height : 0;
    int r_h   = root->right ? root->right->height : 0;

    if(std::abs(l_h - r_h) > 1)
        return false;
    return true;
}

class AvlTree
{
public:

    AvlTree() : root(nullptr) {}

    // recursion
    void Put(int key, int val)
    {
        root = insert(root, key, val);
    }

    //
    bool Get(int key, int& ret)
    {
        auto node = search(root, key);
        if (node) {
            ret = node->val;
            return true;
        }
        return false;
    }

    //
    bool Del(int key)
    {
        root = del(root, key);
        return root != nullptr ? true : false;
    }
// private:
    Node* root;
};

// clang++ -o AvlTree AvlTree.cpp -std=c++17

int main()
{
    AvlTree avlTree;

    for (int i = 0; i < 5; ++i) {
        avlTree.Put(i, i * 2);
        // cout << list.GetCurrentLevel() << endl;
    }
    assert(validateAvLTree(avlTree.root));

    // 测试查找
    for (int i = 0; i < 100; ++i) {
        int value;
        if (avlTree.Get(i, value)) {
            assert(value == i * 2);
        }
    }

    // 测试删除
    for (int i = 0; i < 100; ++i) {
        if (i % 2 == 0) {
            assert(avlTree.Del(i));
        }
    }
    assert(validateAvLTree(avlTree.root));

    return 0;
}