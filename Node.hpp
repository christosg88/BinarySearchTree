#ifndef BINARY_SEARCH_TREE_NODE_HPP
#define BINARY_SEARCH_TREE_NODE_HPP

#include <iostream>
template<typename T>
class Node {
public:
    T value;
    Node<T> *left;
    Node<T> *right;
    Node<T> *parent;
    int height;
    explicit Node(T value);
    ~Node();
    friend std::ostream &operator<<(std::ostream &stream, const Node<T> *node) {
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

template<typename T>
Node<T>::Node(T value) : value(value), left(nullptr), right(nullptr), parent(nullptr), height(0) {
}

template<typename T>
Node<T>::~Node() {
    delete left;
    delete right;
}

#endif
