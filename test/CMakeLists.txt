# Enable google test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB files *.cc **/*.cc)

# Compile the test files
message(${PROJECT_BINARY_DIR})
add_executable(test-procon ${files})
target_link_libraries(test-procon ${GTEST_BOTH_LIBRARIES})
target_link_libraries(test-procon pthread)
add_test(
        NAME google-test-procon
        COMMAND test-procon)
