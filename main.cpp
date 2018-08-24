#include "BinarySearchTree.hpp"

#include <iostream>

int main() {
    BinarySearchTree<int> tree;
    tree.insert(6);
    tree.insert(3);
    tree.insert(2);
    tree.insert(8);
    tree.insert(7);
    tree.insert(9);

    std::cout << tree;
    tree.erase(2);
    std::cout << tree;
    tree.erase(6);
    std::cout << tree;

    return 0;
}
