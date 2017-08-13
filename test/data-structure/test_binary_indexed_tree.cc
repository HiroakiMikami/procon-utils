#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "data-structure/binary_indexed_tree.cc"

TEST(BinaryIndexedTreeTest, ConstructorTest) {
    BIT<int> b({0, 1, 2});
    EXPECT_EQ(3, b.size());
    EXPECT_EQ(0, b.sum(0));
    EXPECT_EQ(0, b.sum(1));
    EXPECT_EQ(1, b.sum(2));
    EXPECT_EQ(3, b.sum(3));

    V<i64> xs = {0, 1, 2};
    BIT<int> b2(CTR(b));
    EXPECT_EQ(3, b2.size());
    EXPECT_EQ(0, b2.sum(0));
    EXPECT_EQ(0, b2.sum(1));
    EXPECT_EQ(1, b2.sum(2));
    EXPECT_EQ(3, b2.sum(3));
}
TEST(BinaryIndexedTreeTest, SumTest) {
    BIT<int> b({1, 3, 2});
    EXPECT_EQ(1, b.sum(0, 1));
    EXPECT_EQ(5, b.sum(1, 3));
    EXPECT_EQ(6, b.sum(0, 3));
}
TEST(BinaryIndexedTreeTest, AtTest) {
    BIT<int> b({1, 3, 2});
    EXPECT_EQ(1, b.at(0));
    EXPECT_EQ(3, b.at(1));
    EXPECT_EQ(2, b.at(2));
}
TEST(BinaryIndexedTreeTest, AddTest) {
    BIT<int> b({1, 3, 2});

    b.add(1, 3);

    EXPECT_EQ(1, b.at(0));
    EXPECT_EQ(6, b.at(1));
    EXPECT_EQ(2, b.at(2));
}
