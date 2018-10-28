#ifndef BINARY_SEARCH_TREE_BINARYSEARCHTREE_HPP
#define BINARY_SEARCH_TREE_BINARYSEARCHTREE_HPP

#include "Node.hpp"

#include <iostream>
#include <stdexcept>
#include <list>
#include <queue>
#include <stack>

template<typename T>
class BinarySearchTree {
    Node<T> *root;
public:
    BinarySearchTree();
    ~BinarySearchTree();
    void insert(T const &value);
    void erase(T const &value);
    friend std::ostream &operator<<(std::ostream &stream, BinarySearchTree<T> const &tree) {
        stream << tree.root << '\n';
        return stream;
    }
    void levelOrder() const;
    std::list<Node<T> *> inOrder() const;
    bool isHeightBalanced() const;
private:
    Node<T> *find(T const &value) const;
    int height(Node<T> const *cursor) const;
    void updateHeights(Node<T> *cursor);
    Node<T> *highestChild(Node<T> const *cursor) const;
    int balanceFactor(Node<T> const *cursor) const;
    void balanceInsertion(Node<T> *leaf);
    void balanceDeletion(Node<T> *cursor);
    void rotateLeft(Node<T> *root);
    void rotateRight(Node<T> *root);
};

template<typename T>
BinarySearchTree<T>::BinarySearchTree() : root(nullptr) {
}

template<typename T>
BinarySearchTree<T>::~BinarySearchTree() {
    delete root;
}

/**
 * Inserts a value to the tree while keeping it balanced
 * @param value The value to be inserted
 */
template<typename T>
void BinarySearchTree<T>::insert(T const &value) {
    if (!root) {
        root = new Node<T>(value);
    }
    else {
        Node<T> *cursor = root;
        while (true) {
            if (cursor->value <= value) {
                if (!cursor->right) {
                    cursor->right = new Node<T>(value);
                    cursor->right->parent = cursor;
                    updateHeights(cursor);
                    balanceInsertion(cursor->right);
                    return;
                }
                cursor = cursor->right;
            }
            else {
                if (!cursor->left) {
                    cursor->left = new Node<T>(value);
                    cursor->left->parent = cursor;
                    updateHeights(cursor);
                    balanceInsertion(cursor->left);
                    return;
                }
                cursor = cursor->left;
            }
        }
    }
}

/**
 * Deletes a value to the tree while keeping it balanced
 * @throw std::invalid_argument() If value isn't present in the tree
 * @param value The value to be deleted
 */
template<typename T>
void BinarySearchTree<T>::erase(T const &value) {
    Node<T> *cursor = find(value);
    if (!cursor) {
        throw std::invalid_argument("Value doesn't exists");
    }
    Node<T> *current = nullptr;
    if (cursor->left && cursor->right) {
        // Node to be deleted has two children

        // Find inorder successor of the node
        Node<T> *inorder_successor = cursor->right;
        while (inorder_successor->left) {
            inorder_successor = inorder_successor->left;
        }

        cursor->value = inorder_successor->value;
        if (inorder_successor->right) {
            current = inorder_successor;
            inorder_successor->value = inorder_successor->right->value;
            delete inorder_successor->right;
            current->right = nullptr;
        }
        else {
            current = inorder_successor->parent;
            delete inorder_successor;
            if (current->left == inorder_successor) {
                current->left = nullptr;
            }
            else {
                current->right = nullptr;
            }
        }

    }
    else if (cursor->left) {
        // Node to be deleted has only one right child
        current = cursor;
        cursor->value = cursor->left->value;
        delete cursor->left;
        current->left = nullptr;
    }
    else if (cursor->right) {
        // Node to be deleted has only one right child
        current = cursor;
        cursor->value = cursor->right->value;
        delete cursor->right;
        current->right = nullptr;
    }
    else {
        // Node to be deleted is leaf
        if (cursor->parent) {
            current = cursor->parent;
            delete cursor;
            if (current->left == cursor) {
                current->left = nullptr;
            }
            else {
                current->right = nullptr;
            }
        }
    }
    updateHeights(current);
    balanceDeletion(current);
}

/**
 * Traverses tree in-order
 * Returns a list of nodes in ascending order
 */
template<typename T>
std::list<Node<T> *> BinarySearchTree<T>::inOrder() const {

    std::list<Node<T> *> in_order;
    Node<T> *cursor = root;
    std::stack<Node<T> *> path;
    do {
        if (!cursor) {
            Node<T> *temp = path.top();
            in_order.emplace_back(temp);
            cursor = temp->right;
            path.pop();
        }
        else {
            path.push(cursor);
            cursor = cursor->left;
        }

    }while (!path.empty() || cursor);

    return in_order;
}

/**
 * Prints nodes from each level of the tree in separate line
 */
template<typename T>
void BinarySearchTree<T>::levelOrder() const {
    std::queue<Node<T> *> unvisited_nodes;
    Node<T> *cursor = root;
    unvisited_nodes.push(cursor);
    size_t len = unvisited_nodes.size();

    while (!unvisited_nodes.empty()) {
        while (len > 0) {
            std::cout << cursor->value << ' ';
            if (cursor->left) {
                unvisited_nodes.push(cursor->left);
            }
            if (cursor->right) {
                unvisited_nodes.push(cursor->right);
            }
            len--;
            unvisited_nodes.pop();
            cursor = unvisited_nodes.front();
        }
        std::cout << '\n';
        len = unvisited_nodes.size();
    }
    std::cout << '\n';
}

template<typename T>
bool BinarySearchTree<T>::isHeightBalanced() const {
    std::list<Node<T> *> in_order_list = inOrder();
    for (Node<T> * const &item:in_order_list) {
        if (balanceFactor(item) > 1 || balanceFactor(item) < -1) {
            return false;
        }
    }
    return true;
}

/**
 * Returns the height of a node
 * if node doesn't exists returns -1
 * @param cursor Node whose height will be returned
 */
template<typename T>
int BinarySearchTree<T>::height(Node<T> const *cursor) const {
    return cursor ? cursor->height:-1;
}

/**
 * Returns a pointer to the first node that has node->value = value
 * if node doesn't exists returns a nullptr
 * @param value The value to compare the nodes to
 */
template<typename T>
Node<T> *BinarySearchTree<T>::find(T const &value) const {
    Node<T> *cursor = root;
    while (cursor) {
        if (cursor->value > value) {
            cursor = cursor->left;
        }
        else if (cursor->value < value) {
            cursor = cursor->right;
        }
        else {
            return cursor;
        }
    }
    return nullptr;
}

/**
 * Updates the heights along the path from cursor to root
 * @param cursor The first node in the path to root
 */
template<typename T>
void BinarySearchTree<T>::updateHeights(Node<T> *cursor) {
    while (cursor) {
        cursor->height = std::max(height(cursor->left), height(cursor->right)) + 1;
        cursor = cursor->parent;
    }
}

/**
 * Returns the balance factor (left subtree height – right subtree height) of cursor.
 * @param cursor The node whose balance factor is returned
 */
template<typename T>
int BinarySearchTree<T>::balanceFactor(Node<T> const *cursor) const{
    return height(cursor->left) - height(cursor->right);
}

/**
 * Performs rotation after a node has been inserted until the tree is balanced
 * @param leaf The first node in the path to root
 */
template<typename T>
void BinarySearchTree<T>::balanceInsertion(Node<T> *leaf) {
    Node<T> *pivot = leaf->parent;
    Node<T> *cursor = pivot->parent;
    while(cursor) {
        int bf = balanceFactor(cursor);
        // if current cursor is unbalanced there are 4 cases
        if (bf > 1) {
            if (pivot->left && pivot->left == leaf) {
                // Left Left Case
                rotateRight(cursor);
                return;
            }
            else {
                // Left Right Case
                rotateLeft(pivot);
                rotateRight(cursor);
                return;
            }
        }
        else if (bf < -1) {
            if (pivot->right && pivot->right == leaf) {
                // Right Right Case
                rotateLeft(cursor);
                return;
            }
            else {
                // Right Left Case
                rotateRight(pivot);
                rotateLeft(cursor);
                return;
            }
        }
        leaf = pivot;
        pivot = cursor;
        cursor = cursor->parent;
    }
}

/**
 * Performs rotation after a node has been deleted until the tree is balanced
 * @param cursor The first node in the path to root
 */
template<typename T>
void BinarySearchTree<T>::balanceDeletion(Node<T> *cursor) {
    while (cursor) {
        int bf = balanceFactor(cursor);
        Node<T> *pivot = highestChild(cursor);

        // if current cursor is unbalanced there are 4 cases
        if (bf > 1) {
            if (balanceFactor(pivot) >= 0) {
                // Left Left Case
                rotateRight(cursor);
            } else {
                // Left Right Case
                rotateLeft(pivot);
                rotateRight(cursor);
            }
        }
        else if (bf < -1) {
            if (balanceFactor(pivot) <= 0) {
                // Right Right Case
                rotateLeft(cursor);
            }
            else {
                // Right Left Case
                rotateRight(pivot);
                rotateLeft(cursor);
            }
        }
        cursor = cursor->parent;
    }
}

/**
 * Returns a pointer to the node that has the larger height from cursor's children
 * @param cursor The node whose children are compared
 */
template<typename T>
Node<T> *BinarySearchTree<T>::highestChild(Node<T> const *cursor) const {
    return (height(cursor->left) > height(cursor->right)) ? cursor->left:cursor->right;
}

/**
 * Left rotates a subtree rooted with root
 * @param root The root of the subtree
 */
template<typename T>
void BinarySearchTree<T>::rotateLeft(Node<T> *root) {
    // root is the initial parent and pivot is the child to take root's place
    Node<T> *pivot = root->right;

    // perform rotation
    root->right = pivot->left;
    if (pivot->left) {
        pivot->left->parent = root;
    }
    pivot->left = root;
    if (root->parent) {
        if (root->parent->left == root) {
            root->parent->left = pivot;
        }
        else {
            root->parent->right = pivot;
        }
    }
    pivot->parent = root->parent;
    root->parent = pivot;

    if (root == this->root) {
        this->root = pivot;
    }
    updateHeights(root);
}

/**
 * Right rotates a subtree rooted with root
 * @param root The root of the subtree
 */
template<typename T>
void BinarySearchTree<T>::rotateRight(Node<T> *root) {
    // root is the initial parent and pivot is the child to take root's place
    Node<T> *pivot = root->left;

    // perform rotation
    root->left = pivot->right;
    if (pivot->right) {
        pivot->right->parent = root;
    }
    pivot->right = root;

    if (root->parent) {
        if (root->parent->left == root) {
            root->parent->left = pivot;
        }
        else {
            root->parent->right = pivot;
        }
    }

    pivot->parent = root->parent;
    root->parent = pivot;

    if (root == this->root) {
        this->root = pivot;
    }
    updateHeights(root);
}
#endif
