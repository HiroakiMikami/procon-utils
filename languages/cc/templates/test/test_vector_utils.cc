#define MAIN

#include <gtest/gtest.h>
#include "common.cc"

TEST(BaseIteratorTest, PreAllocatedVectorTest) {
    auto arr = pre_allocated_vector<size_t>(10);
    EXPECT_EQ(10, arr.capacity());
    EXPECT_EQ(0, arr.size());
}
