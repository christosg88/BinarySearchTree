#ifndef BINARY_SEARCH_TREE_VECTOR_HELPERS_HPP
#define BINARY_SEARCH_TREE_VECTOR_HELPERS_HPP

#include "BinarySearchTree.hpp"

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <type_traits>

/**
 * Generate a vector of ints of size num_nums, in the range [min_val, max_val]
 * @param num_nums The number of values in the returned vector
 * @param min_val The minimum value in the returned vector
 * @param max_val The maximum value in the returned vector
 * @return The vector containing num_nums uniformly distributed random values in the range [min_val, max_val]
 */
template<typename T>
typename std::enable_if<std::is_integral<T>::value, std::vector<T>>::type
generate_random_vector(size_t num_nums, T min_val, T max_val) {
    // create the random device, the generator and the distribution
    std::random_device dev;
    std::mt19937 gen(dev());
    std::uniform_int_distribution<T> dist(min_val, max_val);

    // fill the vector with the random values
    std::vector<T> nums(num_nums);
    std::generate(nums.begin(), nums.end(), [&dist, &gen]() { return dist(gen); });

    return nums;
}

/**
 * Print the values stored in a vector of Nodes
 * @param stream The output stream where the values will be printed
 * @param vec The vector whose values will get printed
 * @return The output stream where the values will have been printed
 */
template<typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<Node<T> *> &vec) {
    for (auto const &item : vec) {
        stream << item->value << " ";
    }
    // delete the last space and enter a new line instead
    stream << "\b\n";
    return stream;
}


/**
 * Print the values stored in a vector of vectors of Nodes
 * @param stream The output stream where the values will be printed
 * @param vec The vector of vectors whose values will get printed
 * @return The output stream where the values will have been printed
 */
template<typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<std::vector<Node<T> *>> &vec) {
    for (auto const &vec_out : vec) {
        for (auto const &item : vec_out) {
            stream << item->value << " ";
        }
        // delete the last space and enter a new line instead
        stream << "\b\n";
    }
    return stream;
}

#endif
