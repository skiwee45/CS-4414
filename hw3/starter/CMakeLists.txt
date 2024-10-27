cmake_minimum_required(VERSION 3.10)

# Set the project name
project(CS4414_HW3)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the optimization level to -O3
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

# Add an executable
add_executable(ptree Gene.cpp hw3.cpp Animal.cpp GeneticTree.cpp)