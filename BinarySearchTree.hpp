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
                // update descendants
                Node<T> *pivot = cursor->parent;
                while(true) {
                    pivot->num_descendants--;
                    if (pivot == this->root) {
                        break;
                    }
                    else {
                        pivot = pivot->parent;
                    }
                }
                delete cursor;
                return;
            }
            else if (!cursor->left) {
                rotateLeft(cursor);
            }
            else if (!cursor->right) {
                rotateRight(cursor);
            }
            else {
                if (cursor->left->num_descendants > cursor->right->num_descendants) {
                    rotateLeft(cursor);
                }
                else {
                    rotateRight(cursor);
                }
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

    // root is the initial parent and pivot is the child to take root's place
    void *rotateLeft(Node<T> *root) {
        Node<T> *pivot = root->right;

        // calculate number of descendants before changing the tree
        root->num_descendants -= pivot->num_descendants + 1;
        if (pivot->left) {
            root->num_descendants += pivot->left->num_descendants + 1;
            pivot->num_descendants -= pivot->left->num_descendants + 1;
        }
        pivot->num_descendants += root->num_descendants + 1;

        // start rotation
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
    }

    void *rotateRight(Node<T> *root) {
        Node<T> *pivot = root->left;

        // calculate number of descendants before changing the tree
        root->num_descendants -= pivot->num_descendants + 1;
        if (pivot->right) {
            root->num_descendants += pivot->right->num_descendants + 1;
            pivot->num_descendants -= pivot->right->num_descendants + 1;
        }
        pivot->num_descendants += root->num_descendants + 1;

        // start rotation
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
    }
};

#endif
