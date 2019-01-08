#include "BinarySearchTree.hpp"
#include "vector_helpers.hpp"
#include "timeit.hpp"

#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>

int main() {

    auto nums = generate_random_vector<int>(10'000'000, -1'000, 1'000);

    ti::ti insertion_timer("Insertion of nodes");
    BinarySearchTree<int> tree;
    for (auto const &item:nums) {
        tree.insert(item);
    }
    insertion_timer.finish();

    ti::ti level_order_timer("Level order traversal");
    auto level_order = tree.levelOrder();
    level_order_timer.finish();

    ti::ti inorder_timer("Inorder traversal");
    std::vector<Node<int> *> inorder = tree.inOrder();
    inorder_timer.finish();


    // create the random device, the generator and the distribution
    std::random_device dev;
    std::mt19937 gen(dev());

    // Rearrange the elements randomly
    std::shuffle(nums.begin(), nums.end(), gen);
    size_t lim = nums.size() / 100;

    ti::ti erase_time("Erase of all nodes");
    for (size_t i = 0; i < lim; ++i) {
        tree.erase(nums[i]);
    }
    erase_time.finish();

}
