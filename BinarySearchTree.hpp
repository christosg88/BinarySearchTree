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
    std::vector<std::vector<Node<T> *>> levelOrder() const;
    std::vector<Node<T> *> inOrder() const;
    std::vector<Node<T> *> postOrder() const;
    std::vector<Node<T> *> preOrder() const;
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

        // Find in order successor of the node
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
 * Traverses tree in-order (Left, Root, Right)
 * @return a vector of nodes in ascending order
 */
template<typename T>
std::vector<Node<T> *> BinarySearchTree<T>::inOrder() const {
    Node<T> *cursor = root;
    std::stack<Node<T> *> path;

    std::vector<Node<T> *> in_order;
    while (!path.empty() || cursor) {
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
    }

    return in_order;
}

/**
 * Traverses tree in post-order (Left, Right, Root)
 * @return a vector of nodes
 */
template<typename T>
std::vector<Node<T> *> BinarySearchTree<T>::postOrder() const {
    std::stack<Node<T> *> path;
    path.push(root);

    std::deque<Node<T> *> post_order;
    while (!path.empty()) {
        Node<T> * cursor = path.top();
        path.pop();
        post_order.push_front(cursor);

        if (cursor->left) {
            path.push(cursor->left);
        }
        if (cursor->right) {
            path.push(cursor->right);
        }
    }

    return std::vector<Node<T> *>(post_order.begin(), post_order.end());
}

/**
 * Traverses tree in pre-order (Root, Left, Right)
 * @return a vector of nodes
 */
template<typename T>
std::vector<Node<T> *> BinarySearchTree<T>::preOrder() const {
    std::stack<Node<T> *> path;
    path.push(root);

    std::vector<Node<T> *> pre_order;
    while (!path.empty()) {
        Node<T> *cursor = path.top();
        path.pop();
        pre_order.emplace_back(cursor);

        if (cursor->right) {
            path.push(cursor->right);
        }
        if (cursor->left) {
            path.push(cursor->left);
        }
    }
    return pre_order;
}

/**
 *  Traverses tree in level order (from left to right, level by level).
 *  @return a vector of vectors with nodes.
 */
template<typename T>
std::vector<std::vector<Node<T> *>> BinarySearchTree<T>::levelOrder() const {

    std::vector<std::vector<Node<T> *>> level_order;
    std::deque<Node<T> *> q;

    if (root){
        q.push_back(root);
    }

    while (!q.empty()) {
        size_t size = q.size();
        std::vector<Node<T> *> inner_level_order;

        while (size--) {
            Node<T> *cursor = q.front();
            q.pop_front();
            inner_level_order.push_back(cursor);
            if (cursor->left){
                q.push_back(cursor->left);
            }
            if (cursor->right) {
                q.push_back(cursor->right);
            }
        }

        level_order.push_back(inner_level_order);
    }

    return level_order;
}

/**
 *  Traverses the tree the same way as level order (BFS) and checks if every node
 *  is height balanced (the depth of the two subtrees of every node never differ by more than 1)
 *  @return true if it is balanced, false if not
 */
template<typename T>
bool BinarySearchTree<T>::isHeightBalanced() const {
    std::deque<Node<T> *> q;

    if (root){
        q.push_back(root);
    }

    while (!q.empty()) {
        size_t size = q.size();
        while (size--) {
            Node<T> *cursor = q.front();
            q.pop_front();

            if (balanceFactor(cursor) > 1 || balanceFactor(cursor) < -1) {
                return false;
            }

            if (cursor->left){
                q.push_back(cursor->left);
            }
            if (cursor->right) {
                q.push_back(cursor->right);
            }
        }
    }


    return true;
}

/**
 * @param cursor Node whose height will be returned
 * @return the height of the node if the node doesn't exists returns -1
 */
template<typename T>
int BinarySearchTree<T>::height(Node<T> const *cursor) const {
    return cursor ? cursor->height:-1;
}

/**
 * @param value The value to compare the nodes to
 * @return a pointer to the first node that has node->value = value or nullptr if it doesn't exists
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
 * @param cursor The node whose balance factor is returned
 * @return the balance factor (left subtree height – right subtree height) of cursor.
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
 * @param cursor The node whose children are compared
 * @return a pointer to the node that has the larger height from cursor's children
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
