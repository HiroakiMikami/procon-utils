cmake_minimum_required(VERSION 3.7)
project(atcoder-workspace)

# Common settings for the project
enable_testing()

# Common C++ settings
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++14)

# Add subdirectories
add_subdirectory(template)
add_subdirectory(contest)
