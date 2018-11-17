cmake_minimum_required(VERSION 3.10)
project(Binary_Search_Tree)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -ggdb -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Werror -O3")
set(HDR_FILES Node.hpp BinarySearchTree.hpp)


add_executable(Binary_Search_Tree main.cpp ${HDR_FILES})
