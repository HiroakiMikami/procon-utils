#define MAIN

#include <gtest/gtest.h>
#include "common.cc"

TEST(VectorUtilsTest, PreAllocatedVectorTest) {
    auto arr = make_pre_allocated_vector<size_t>(10);
    EXPECT_EQ(10, arr.capacity());
    EXPECT_EQ(0, arr.size());
}

TEST(VectorUtilsTest, MultiDimentionalArrayTest) {
    auto mat = make_matrix<int, 2>({1, 2});

    EXPECT_EQ(1, mat.size());
    EXPECT_EQ(2, mat[0].size());
    EXPECT_EQ(0, mat[0][0]);
    EXPECT_EQ(0, mat[0][1]);


    auto mat2 = make_matrix<int, 0>({}, 10);
    EXPECT_EQ(10, mat2);

    auto mat3 = make_matrix<int, 1>({1}, 10);
    EXPECT_EQ(10, mat3[0]);
}
