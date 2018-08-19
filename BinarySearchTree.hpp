#ifndef BINARY_SEARCH_TREE_BINARYSEARCHTREE_HPP
#define BINARY_SEARCH_TREE_BINARYSEARCHTREE_HPP

#include "Node.hpp"

#include <iostream>
#include <stdexcept>

template<typename T>
class BinarySearchTree {
    Node<T> *root;

public:
    BinarySearchTree() : root(nullptr) {
    }

    ~BinarySearchTree() {
        delete root;
    }

    void insert(T const &value) {
        if (root == nullptr) {
            root = new Node<T>(value);
        }
        else {
            Node<T> *cursor = root;
            while (true) {
                cursor->num_descendants++;
                if (cursor->value <= value) {
                    if (cursor->right == nullptr) {
                        cursor->right = new Node<T>(value);
                        cursor->right->parent = cursor;
                        return;
                    }
                    cursor = cursor->right;
                }
                else {
                    if (cursor->left == nullptr) {
                        cursor->left = new Node<T>(value);
                        cursor->left->parent = cursor;
                        return;
                    }
                    cursor = cursor->left;
                }
            }
        }
    }

    void erase(T const &value) {
        Node<T> *cursor = find(value);
        if (!cursor) {
            throw std::invalid_argument("Value doesn't exists");
        }
        while (true) {
            if (!cursor->num_descendants) {
                if (cursor->parent) {
                    if (cursor->parent->left == cursor) {
                        cursor->parent->left = nullptr;
                    }
                    else {
                        cursor->parent->right = nullptr;
                    }
                }
                delete cursor;
                return;
            }
            else if (!cursor->left) {
                cursor = rotateLeft(cursor);
            }
        }

    }

    friend std::ostream &operator<<(std::ostream &stream, BinarySearchTree<T> const &tree) {
        stream << tree.root << '\n';
        return stream;
    }

private:
    Node<T> *find(T const &value) {
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

    Node<T> *rotateLeft(Node<T> *root) {
        Node<T> *pivot = root->right;
        root->right = pivot->left;
        pivot->left->parent = root;
        pivot->left = root;
        root->parent = pivot;
        return pivot;
    }

    Node<T> *rotateRight(Node<T> *root) {
        Node<T> *pivot = root->left;
        root->left = pivot->right;
        pivot->right->parent = root;
        pivot->right = root;
        root->parent = pivot;
        return pivot;
    }
};

#endif
