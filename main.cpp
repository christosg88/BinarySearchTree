#include "BinarySearchTree.hpp"

#include <iostream>

int main() {
    BinarySearchTree<int> tree;
    tree.insert(50);
    tree.insert(25);
    tree.insert(75);
    tree.insert(10);
    tree.insert(60);
    tree.insert(30);
    tree.insert(80);
    tree.insert(5);
    tree.insert(55);
    tree.insert(15);
    tree.insert(27);
    tree.insert(1);
    tree.levelOrder();
    tree.erase(80);
    tree.levelOrder();
    return 0;
}
