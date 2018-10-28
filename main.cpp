#include "BinarySearchTree.hpp"

#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>

int main() {
    const int NUM_NUMS = 1000;
    const int MIN_VAL = -10000, MAX_VAL = 10000;

    // fill a vector with NUM_NUMS random numbers in the range [MIN_VAL, MAX_VAL] that follow a uniform distribution
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<int> dist(MIN_VAL, MAX_VAL);
    std::vector<int> nums(NUM_NUMS);
    std::generate(nums.begin(), nums.end(), [&dist, &gen]() {
        return dist(gen);
    });

    BinarySearchTree<int> tree;
    for (auto const &item:nums) {
        tree.insert(item);
    }

    // Rearrange the elements randomly
    std::shuffle(nums.begin(), nums.end(), gen);
    int lim = NUM_NUMS / 100;
    for (int i = 0; i < lim; ++i) {
        if (!tree.isHeightBalanced()) {
            std::cout << "Tree is unbalanced!";
            break;
        }
        tree.erase(nums[i]);
    }
}
