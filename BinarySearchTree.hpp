#ifndef BINARY_SEARCH_TREE_BINARYSEARCHTREE_HPP
#define BINARY_SEARCH_TREE_BINARYSEARCHTREE_HPP

#include "Node.hpp"

#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <queue>

template<typename T>
class BinarySearchTree {
    Node<T> *root;

public:
    BinarySearchTree() : root(nullptr) {
    }

    ~BinarySearchTree() {
        delete root;
    }
    void insert(T const &value);
    void erase(T const &value);
    friend std::ostream &operator<<(std::ostream &stream, BinarySearchTree<T> const &tree);
    void levelOrder() const;
    void inOrder() const;
private:
    Node<T> *find(T const &value) const;
    int height(Node<T> *cursor) const;
    void updateHeights(Node<T> *cursor);
    Node<T> *highestChild(Node<T> *cursor) const;
    int balanceFactor(Node<T> *cursor);
    void balanceInsertion(Node<T> *leaf);
    void balanceDeletion(Node<T> *cursor);
    void *rotateLeft(Node<T> *root);
    void *rotateRight(Node<T> *root);
    void inOrder(Node<T> *root) const;
};

/**
 * Inserts a value to the tree while keeping it balanced
 * @param value The value to be inserted
 */
template<typename T>
void BinarySearchTree<T>::insert(const T &value) {
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

    if (cursor->left && cursor->right) {
        // Deleting a node with two children

        // in-order successor node as replacement node
        Node<T> *in_order_successor = cursor->right;
        while (true) {
            if (in_order_successor->left) {
                in_order_successor = in_order_successor->left;
            }
            else {
                break;
            }
        }

        cursor->value = in_order_successor->value;
        if (!in_order_successor->right) {
            // If in_order_successor does not have a child
            // remove in_order_successor from its parent
            if (in_order_successor->parent->left == in_order_successor) {
                in_order_successor->parent->left = nullptr;
            }
            else {
                in_order_successor->parent->right = nullptr;
            }
            updateHeights(in_order_successor->parent);
            balanceDeletion(in_order_successor->parent);
            delete in_order_successor;
        }
        else {
            // If in_order_successor has a child, it is a right child.
            // Replace in_order_successor with right child at in_order_successor's parent.
            if (in_order_successor->parent->left == in_order_successor) {
                in_order_successor->parent->left = in_order_successor->right;
            }
            else {
                in_order_successor->parent->right = in_order_successor->right;
            }
            updateHeights(in_order_successor->right);
            balanceDeletion(in_order_successor->right);
            delete in_order_successor;
        }
    }
    else if (cursor->left) {
        // Deleting a node with only left child
        // update parent
        if (cursor->parent) {
            if (cursor->parent->left == cursor) {
                cursor->parent->left = cursor->left;
            }
            else {
                cursor->parent->right = cursor->left;
            }
        }
        updateHeights(cursor->parent);
        balanceDeletion(cursor->parent);
        delete cursor;
        return;
    }
    else if (cursor->right) {
        // Deleting a node with only right child
        // update parent
        if (cursor->parent) {
            if (cursor->parent->left == cursor) {
                cursor->parent->left = cursor->right;
            }
            else {
                cursor->parent->right = cursor->right;
            }
        }
        updateHeights(cursor->parent);
        balanceDeletion(cursor->parent);
        delete cursor;
    }
    else {
        // Deleting a node with no children
        // update parent
        if (cursor->parent) {
            if (cursor->parent->left == cursor) {
                cursor->parent->left = nullptr;
            }
            else {
                cursor->parent->right = nullptr;
            }
        }
        updateHeights(cursor->parent);
        balanceDeletion(cursor->parent);
        delete cursor;
    }
}

template<typename T>
std::ostream &operator<<(std::ostream &stream, const BinarySearchTree<T> &tree) {
    stream << tree.root << '\n';
    return stream;
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

/**
 * Traverses tree in-order
 * Prints nodes in ascending order
 */
template<typename T>
void BinarySearchTree<T>::inOrder() const {
    inOrder(root);
    std::cout << '\n';
}

/**
 * Returns the height of a node
 * if node doesn't exists returns -1
 * @param cursor Node whose height will be returned
 */
template<typename T>
int BinarySearchTree<T>::height(Node<T> *cursor) const {
    if (cursor) {
        return cursor->height;
    }
    return -1;
}

/**
 * Returns a pointer to the first node that has node->value = value
 * if node doesn't exists returns a nullptr
 * @param value The value to compare the nodes to
 */
template<typename T>
Node<T> *BinarySearchTree<T>::find(const T &value) const {
    Node<T> *cursor = root;
    while (true) {
        if (!cursor) {
            return nullptr;
        }
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
}

/**
 * Updates the heights along the path from cursor to root
 * @param cursor The first node in the path to root
 */
template<typename T>
void BinarySearchTree<T>::updateHeights(Node<T> *cursor) {
    Node<T> *temp = cursor;
    while (temp) {
        temp->height = std::max(height(temp->left), height(temp->right)) + 1;
        temp = temp->parent;
    }
}

/**
 * Returns the balance factor (left subtree height – right subtree height) of cursor.
 * @param cursor The node whose balance factor is returned
 */
template<typename T>
int BinarySearchTree<T>::balanceFactor(Node<T> *cursor) {
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
    if (!cursor) {
        return;
    }

    int bf;
    while(cursor) {
        bf = balanceFactor(cursor);
        // current cursor is unbalanced.
        // Left Left case or Left Right case.
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
        // current node is unbalanced.
        // Right Right case or Right-Left case.
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
    int bf;
    Node<T> *pivot;
    Node<T> *leaf;
    while (cursor) {
        bf = balanceFactor(cursor);
        pivot = highestChild(cursor);   // the larger height child of cursor
        leaf = highestChild(pivot);     // the larger height child of pivot

        // current cursor is unbalanced.
        // Left Left case or Left Right case.
        if (bf > 1) {
            if (pivot->left && pivot->left == leaf) {
                // Left Left Case
                rotateRight(cursor);
            }
            else {
                // Left Right Case
                rotateLeft(pivot);
                rotateRight(cursor);
            }
        }
        // current node is unbalanced.
        // Right Right case or Right-Left case.
        else if (bf < -1) {
            if (pivot->right && pivot->right == leaf) {
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
Node<T> *BinarySearchTree<T>::highestChild(Node<T> *cursor) const {
    if (cursor) {
        if (height(cursor->left) > height(cursor->right)) {
            return cursor->left;
        }
        else if (cursor->right){
            return cursor->right;
        }
    }
    return nullptr;
}

/**
 * Left rotates a subtree rooted with root
 * @param root The root of the subtree
 */
template<typename T>
void *BinarySearchTree<T>::rotateLeft(Node<T> *root) {
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
void *BinarySearchTree<T>::rotateRight(Node<T> *root) {
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

template<typename T>
void BinarySearchTree<T>::inOrder(Node<T> *root) const {
    if (root) {
        if (root->left) {
            inOrder(root->left);
        }
        std::cout << root->value << ' ';
        if (root->right) {
            inOrder(root->right);
        }
    }
}

#endif
