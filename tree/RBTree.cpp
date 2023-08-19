// https://www.jianshu.com/p/e136ec79235c

#include <atomic>
#include <cassert>
#include <filesystem>
#include <iostream>
#include <ostream>

enum class Color { RED = 0, BLACK = 1 };

struct Node {
	int key;
	int val;

	Color color;
	Node *parent;
	Node *left, *right;

	Node(int k, int v) : key(k), val(v), color(Color::RED), parent(nullptr), left(nullptr), right(nullptr) {
	}
};

// std::ostream & operator<< (std::ostream  &os, const Node &node)
// {
//     os << "(" << node.key << "," << node.val << ")";
//        <<
// }

class RBtree {
private:
	//     |                       |
	//     N                       S
	//    / \     l-rotate(N)     / \
    //   L   S    ==========>    N   R
	//      / \                 / \
    //     M   R               L   M
	void leftRotate(Node *p) {
		if (p) {
			Node *r = p->right;
			p->right = r->left;

			if (r->left)
				r->left->parent = p;

			r->parent = p->parent;

			if (p->parent == nullptr) {
				root = r;
			} else if (p == p->parent->left) {
				p->parent->left = r;
			} else {
				p->parent->right = r;
			}
			r->left = p;
			p->parent = r;
		}
	}

	//       |                   |
	//       N                   S
	//      / \   r-rotate(N)   / \
    //     S   R  ==========>  L   N
	//    / \                     / \
    //   L   M                   M   R
	void rightRotate(Node *p) {
		if (p) {
			Node *r = p->left;
			p->left = r->right;

			if (r->right)
				r->right->parent = p;

			r->parent = p->parent;

			if (p->parent == nullptr) {
				root = r;
			} else if (p == p->parent->left) {
				p->parent->left = r;
			} else {
				p->parent->right = r;
			}
			r->right = p;
			p->parent = r;
		}
	}

	/**
	 * Returns the successor of the specified Node, or null if no such.
	 */
	Node *successor(Node *t) {
		if (t == nullptr) {
			return nullptr;
		} else if (t->right) {
			Node *p = t->right;
			while (p->left)
				p->left;
			return p;
		} else {
			Node *p = t->parent;
			Node *ch = t;
			while (p && ch == p->right) {
				ch = p;
				p = p->parent;
			}
			return p;
		}
	}

	/**
	 * Returns the predecessor of the specified Entry, or null if no such.
	 */
	Node *predecessor(Node *t) {
		if (t == nullptr) {
			return nullptr;
		} else if (t->left) {
			Node *p = t->left;
			while (p->right)
				p->right;
			return p;
		} else {
			Node *p = t->parent;
			Node *ch = t;
			while (p && ch == p->left) {
				ch = p;
				p = p->parent;
			}
			return p;
		}
	}

	void fixAfterInsertion(Node *node) {
		// TODO
		while (node && node != root && node->parent->color == Color::RED) {
			auto g = node->parent->parent;

			if (node->parent == g->left) {
				auto u = g->right;
				//        |
				//       G(B)
				//      /   \  
                //    P(R)  U(R)
				//    /
				//  I(R)
				if (u && u->color == Color::RED) {
					node->parent->color = Color::BLACK;
					u->color = Color::BLACK;
					g->color = Color::RED;
					node = g;
				} else {
					//        |
					//       G(B)
					//      /   \  
                    //    P(R)  U(B) | NULL
					//    /
					//  I(R)
					if (node == node->parent->right) {
						node = node->parent;
						leftRotate(node);
					}

					//       |                              |
					//      G(B)                           P(B)
					//      / \           r-rotate(N)      / \
                    //   P(R)  U(B)| NULL ==========>   I(R)  G(R)
					//    /                                   / \
                    // I(R)                                     U(B) | NULL
					rightRotate(g);
					node->parent->color = Color::BLACK;
					g->color = Color::RED;
				}
			} else {
				auto u = g->left;
				if (u && u->color == Color::RED) {
					node->parent->color = Color::BLACK;
					u->color = Color::BLACK;
					g->color = Color::RED;
					node = g;
				} else {
					if (node->parent->left == node) {
						node = node->parent;
						rightRotate(node);
					}
					leftRotate(g);
					node->parent->color = Color::BLACK;
					g->color = Color::RED;
				}
			}

			root->color = Color::BLACK;
		}
	}

	void fixAfterDeletion(Node *node) {
		while (node != root && node->color == Color::BLACK) {
			if (node == node->parent->left) {
				Node *sib = node->parent->right;
				//     |                       |
				//    P(B)                    S(B)
				//    / \     l-rotate(N)     /  \
                // L(B) S(R)   ========>    P(R) SR(B)
				//      / \                 / \ 
                //  SL(B) SR(B)          L(B)  SL(B)
				if (sib->color == Color::RED) {
					sib->color = Color::BLACK;
					node->parent->color = Color::RED;
					leftRotate(node->parent);
					sib = node->parent->right;
				}

				//     |                       |
				//   P(B|R)                   S(B|R)
				//    / \     l-rotate(N)     /  \
                // L(B) S(B)   ========>    L(B) S(R)
				//      / \                       / \ 
                //  SL(B) SR(B)                SL(B) SR(B)
				if (sib->left->color == Color::BLACK && sib->right->color == Color::BLACK) {
					sib->color = Color::RED;
					node = node->parent;
				} else {
					//     |                         |
					//   P(B|R)                    P(B|R)
					//    / \     l-rotate(N)       /  \
                    // L(B) S(B)   ========>      L(B) SL(B)
					//      / \                          \
                    //  SL(R) SR(B)                      S(R)
					//                                     \
                    //                                     SR(B)
					if (sib->right->color == Color::BLACK) {
						sib->left->color = Color::BLACK;
						sib->color = Color::RED;
						rightRotate(sib);
						sib = node->parent->right;
					}
					//     |                         |
					//   P(B|R)                    S(B|R)
					//    / \     l-rotate(N)       /  \
                    // L(B) S(B)   ========>     P(B)  SR(B)
					//      / \                  /  \     
                    //  SL(B|R) SR(R)          L(B) SL(B|R)
					sib->color = node->parent->color;
					node->parent->color = Color::BLACK;
					sib->right->color = Color::BLACK;
					leftRotate(node->parent);
					node = root;
				}
			} else {
				Node *sib = node->parent->left;
				if (sib->color == Color::RED) {
					sib->color = Color::BLACK;
					node->parent->color = Color::RED;
					rightRotate(node->parent);
					sib = node->parent->left;
				}

				if (sib->left->color == Color::BLACK && sib->right->color == Color::BLACK) {
					sib->color = Color::RED;
					node = node->parent;
				} else {
					if (sib->left->color == Color::BLACK) {
						sib->right->color = Color::BLACK;
						sib->color = Color::RED;
						rightRotate(sib);
						sib = node->parent->left;
					}
					sib->color = node->parent->color;
					node->parent->color = Color::BLACK;
					sib->left->color = Color::BLACK;
					leftRotate(node->parent);
					node = root;
				}
			}
		}
		node->color = Color::BLACK;
	}

public:
	RBtree() : root(nullptr) {};

	void Put(int key, int val) {
		auto node = new Node(key, val);
		if (root == nullptr) {
			root = node;
			root->color = Color::BLACK;
			return;
		}
		auto t = root;
		Node *parent = nullptr;
		while (t) {
			parent = t;
			if (t->key == key) { // exsit
				t->val = val;
				return;
			}
			t = t->key < key ? t->right : t->left;
		}
		node->parent = parent;
		if (parent->key < key)
			parent->right = node;
		else
			parent->left = node;
		if (node->parent->color == Color::BLACK)
			return;

		fixAfterInsertion(node);
	}

	bool Get(int key, int &val) {
		Node *p = root;
		while (p) {
			if (p->key == key) {
				val = p->val;
				return true;
			}
			p = p->key < key ? p->right : p->left;
		}
		return false;
	}

	//
	bool Del(int key) {
		Node *p = root;
		while (p) {
			if (p->key == key)
				break;
			p = p->key < key ? p->right : p->left;
		}
		if (p == nullptr)
			return false;
		// If strictly internal, copy successor's element to p and then make p
		// point to successor.
		if (p->left != nullptr && p->right != nullptr) {
			Node *s = successor(p);
			p->key = s->key;
			p->val = s->val;
			p = s;
		} // p has 2 children

		//       |
		//      6(B)
		//      / \ 
        //   5(B)  7(B)
		//    /
		// 3(R)

		// Start fixup at replacement node, if it exists.
		Node *replacement = (p->left != nullptr ? p->left : p->right);

		if (replacement != nullptr) {
			// Link replacement to parent
			replacement->parent = p->parent;
			if (p->parent == nullptr)
				root = replacement;
			else if (p == p->parent->left)
				p->parent->left = replacement;
			else
				p->parent->right = replacement;

			// Null out links so they are OK to use by fixAfterDeletion.
			p->left = p->right = p->parent = nullptr;

			// Fix replacement
			if (p->color == Color::BLACK)
				fixAfterDeletion(replacement);
		} else if (p->parent == nullptr) { // return if we are the only node.
			root = nullptr;
		} else { //  No children. Use self as phantom replacement and unlink.
			if (p->color == Color::BLACK)
				fixAfterDeletion(p);

			if (p->parent != nullptr) {
				if (p == p->parent->left)
					p->parent->left = nullptr;
				else if (p == p->parent->right)
					p->parent->right = nullptr;
				p->parent = nullptr;
			}
		}
		return true;
	}

	Node *root;
};

// clang++ -o RBtree RBtree.cpp -std=c++17

int main() {
	RBtree rbTree;
	rbTree.Put(5, 2);
	rbTree.Put(6, 3);
	rbTree.Put(7, 4);
	assert(rbTree.root->key == 6 && rbTree.root->color == Color::BLACK);
	assert(rbTree.root->left->key == 5 && rbTree.root->left->color == Color::RED);
	assert(rbTree.root->right->key == 7 && rbTree.root->right->color == Color::RED);

	rbTree.Put(3, 3);
	//       |
	//      6(B)
	//      / \ 
    //   5(B)  7(B)
	//    /
	// 3(R)
	assert(rbTree.root->key == 6 && rbTree.root->color == Color::BLACK);
	assert(rbTree.root->left->key == 5 && rbTree.root->left->color == Color::BLACK);
	assert(rbTree.root->right->key == 7 && rbTree.root->right->color == Color::BLACK);
	assert(rbTree.root->left->left->key == 3 && rbTree.root->left->left->color == Color::RED);

	rbTree.Put(4, 4);
	//       |
	//      6(B)
	//      / \ 
    //   4(B)  7(B)
	//    / \
    // 3(R)  5(R)
	assert(rbTree.root->key == 6 && rbTree.root->color == Color::BLACK);
	assert(rbTree.root->left->key == 4 && rbTree.root->left->color == Color::BLACK);
	assert(rbTree.root->right->key == 7 && rbTree.root->right->color == Color::BLACK);
	assert(rbTree.root->left->left->key == 3 && rbTree.root->left->left->color == Color::RED);
	assert(rbTree.root->left->right->key == 5 && rbTree.root->left->right->color == Color::RED);

	assert(rbTree.Del(4));
	//       |
	//      6(B)
	//      / \ 
    //   5(B)  7(B)
	//    /
	// 3(R)
	assert(rbTree.root->key == 6 && rbTree.root->color == Color::BLACK);
	assert(rbTree.root->left->key == 5 && rbTree.root->left->color == Color::BLACK);
	assert(rbTree.root->right->key == 7 && rbTree.root->right->color == Color::BLACK);
	assert(rbTree.root->left->left->key == 3 && rbTree.root->left->left->color == Color::RED);

	assert(rbTree.Del(5));
	//       |
	//      6(B)
	//      / \ 
    //   5(B)  7(B)
	assert(rbTree.root->key == 6 && rbTree.root->color == Color::BLACK);
	assert(rbTree.root->left->key == 3 && rbTree.root->left->color == Color::BLACK);
	assert(rbTree.root->right->key == 7 && rbTree.root->right->color == Color::BLACK);
	// std::cout << (node->parent->color == Color::BLACK ? "black" : "red") << "\n";

	return 0;
}