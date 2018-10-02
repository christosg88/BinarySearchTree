#ifndef BINARY_SEARCH_TREE_NODE_HPP
#define BINARY_SEARCH_TREE_NODE_HPP

#include <iostream>
#include <cstddef>

template<typename T>
class Node {
public:
    T value;
    Node<T> *left;
    Node<T> *right;
    Node<T> *parent;
    int height;

    explicit Node(T value) : value(value), left(nullptr), right(nullptr), parent(nullptr), height(0) {
    }

    ~Node() {
        delete left;
        delete right;
    }

    friend std::ostream &operator<<(std::ostream &stream, Node<T> const *node) {
        if (node->left) {
            stream << node->left;
        }

        stream << node->value << ' ';

        if (node->right) {
            stream << node->right;
        }

        return stream;
    }
};

#endif
