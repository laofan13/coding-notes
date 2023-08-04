#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>

static constexpr size_t ALPHABETS = 26;

struct TrieNode
{
    // An array of pointers of size 26 which tells if a character of word is
    // present or not.
    std::shared_ptr<TrieNode> character[ALPHABETS]{ nullptr };
    // std::array<std::shared_ptr<trie>, NUM_CHARS << 1> arr;

    bool isEndOfWord{ false };
};

inline static bool hasChildren(std::shared_ptr<TrieNode> node)
{
    for (size_t i = 0; i < ALPHABETS; i++) {
        if (node->character[i]) {
            return true;
        }
    }
    return false;
}

static std::shared_ptr<TrieNode> removeWord(
    const std::string& word,
    std::shared_ptr<TrieNode> curr,
    size_t index) {

    if (word.size() == index) {
        if (curr->isEndOfWord) {
            curr->isEndOfWord = false;
        }
        if (hasChildren(curr)) {
            return curr;
        }
        return nullptr;
    }
    // std::cout << index << std::endl;

    size_t idx = word[index] - 'a';
    
    assert(curr->character[idx]);

    curr->character[idx] = removeWord(word, curr->character[idx], index + 1);

    if (curr->character[idx] || hasChildren(curr)) {
        return curr;
    }
    return nullptr;

}

class TrieTree
{
public:
    TrieTree() : root(std::make_shared<TrieNode>()) {
        
    }

    void Put(const std::string& word)
    {
        auto curr = root;
        for (char ch : word) {
            size_t index = ch - 'a';

            // if a node for current word is not already present in trie, create
            // a new node for it.
            if (!curr->character[index]) {
                curr->character[index] = std::make_shared<TrieNode>();
            }

            curr = curr->character[index];
        }
        curr->isEndOfWord = true;
    }

    bool Get(const std::string& word) {
        auto curr = root;
        for (char ch : word) {
            size_t index = ch - 'a';

            // if any node for a character is not found, then return that the
            // word cannot be formed.
            if (!curr->character[index]) {
                return false;
            }
            curr = curr->character[index];
        }
        return curr->isEndOfWord;
    }

    void Del(const std::string& word) {
        root = removeWord(word, root, 0);
    }

private:
    std::shared_ptr<TrieNode> root;
};

// clang++ -o TrieTree TrieTree.cpp -std=c++17

int main()
{
    TrieTree trie;
    trie.Put("hel");
    trie.Put("hello");
    trie.Put("hel");
    assert(trie.Get("hello"));
    assert(trie.Get("hel"));
    trie.Del("hel");
    assert(trie.Get("hello"));
    assert(!trie.Get("hel"));
    return 0;
}