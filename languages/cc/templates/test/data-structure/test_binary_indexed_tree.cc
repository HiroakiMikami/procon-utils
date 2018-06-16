#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "data-structure/binary_indexed_tree.cc"

TEST(BinaryIndexedTreeTest, ConstructorTest) {
    BinaryIndexedTree<int> b({0, 1, 2});
    EXPECT_EQ(3, b.size());
    EXPECT_EQ(0, b.sum(0));
    EXPECT_EQ(0, b.sum(1));
    EXPECT_EQ(1, b.sum(2));
    EXPECT_EQ(3, b.sum(3));

    Vector<i64> xs = {0, 1, 2};
    BinaryIndexedTree<int> b2(CTR(b));
    EXPECT_EQ(3, b2.size());
    EXPECT_EQ(0, b2.sum(0));
    EXPECT_EQ(0, b2.sum(1));
    EXPECT_EQ(1, b2.sum(2));
    EXPECT_EQ(3, b2.sum(3));
}
TEST(BinaryIndexedTreeTest, SumTest) {
    BinaryIndexedTree<int> b({1, 3, 2});
    EXPECT_EQ(1, b.sum(0, 1));
    EXPECT_EQ(5, b.sum(1, 3));
    EXPECT_EQ(6, b.sum(0, 3));
}
TEST(BinaryIndexedTreeTest, AtTest) {
    BinaryIndexedTree<int> b({1, 3, 2});
    EXPECT_EQ(1, b.at(0));
    EXPECT_EQ(3, b.at(1));
    EXPECT_EQ(2, b.at(2));
}
TEST(BinaryIndexedTreeTest, AddTest) {
    BinaryIndexedTree<int> b({1, 3, 2});

    b.add(1, 3);

    EXPECT_EQ(1, b.at(0));
    EXPECT_EQ(6, b.at(1));
    EXPECT_EQ(2, b.at(2));
}

TEST(BinaryIndexedTreeTest, OutputTest) {
    BinaryIndexedTree<int> b({1, 3, 2});
    testing::internal::CaptureStderr();
    dump(b);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("[1, 3, 2]\n"), output);
}