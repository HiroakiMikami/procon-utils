cmake_minimum_required(VERSION 3.7)
project(procon-library)

# Common settings for the project
enable_testing()

# C++ settings
set(CMAKE_CXX_STANDARD  14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -D_GLIBCXX_DEBUG")
include_directories(${PROJECT_SOURCE_DIR}/languages/cc/templates/src)

# Add subdirectories
add_subdirectory(${PROJECT_SOURCE_DIR}/languages/cc/templates/test)
