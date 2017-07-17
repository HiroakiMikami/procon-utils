cmake_minimum_required(VERSION 3.7)
project(procon-library)

# Common settings for the project
enable_testing()

# C++ settings
set(CMAKE_CXX_STANDARD  14)
include_directories(${PROJECT_SOURCE_DIR}/src)

# Add subdirectories
add_subdirectory(test)
