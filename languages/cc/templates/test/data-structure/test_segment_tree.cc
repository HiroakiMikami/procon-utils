#define MAIN

#include <gtest/gtest.h>
#include <numeric>
#include "common.cc"
#include "data-structure/segment_tree.cc"

TEST(RangeMinimumQueryTest, QueryTest) {
    RangeMinimumQuery<int> rmq(Vector<int>{0, 1, 2, 3});
    EXPECT_EQ(4, rmq.size());
    EXPECT_EQ(0, rmq.query(0, 1));
    EXPECT_EQ(1, rmq.query(1, 2));
    EXPECT_EQ(2, rmq.query(2, 3));
    EXPECT_EQ(3, rmq.query(3, 4));

    EXPECT_EQ(0, rmq.query(0, 2));
    EXPECT_EQ(1, rmq.query(1, 3));
    EXPECT_EQ(2, rmq.query(2, 4));

    EXPECT_EQ(0, rmq.query(0, 3));
    EXPECT_EQ(1, rmq.query(1, 4));

    EXPECT_EQ(0, rmq.query(0, 4));
}
TEST(RangeMinimumQueryTest, QueryWithNonPowerOf2Elems) {
    RangeMinimumQuery<int> rmq(Vector<int>{0, 1, 2});
    EXPECT_EQ(3, rmq.size());
    EXPECT_EQ(2, rmq.query(2, 3));
    EXPECT_EQ(2, rmq.query(2, 4));

    EXPECT_THROW(rmq.query(3, 4), std::out_of_range);
}
TEST(RangeMinimumQueryTest, UpdateTest) {
    RangeMinimumQuery<int> rmq(Vector<int>{0, 1, 2, 3});
    rmq.update(0, 10);
    EXPECT_EQ(10, rmq.query(0, 1));
    EXPECT_EQ(1, rmq.query(0, 2));
    EXPECT_EQ(1, rmq.query(0, 3));
    EXPECT_EQ(1, rmq.query(0, 4));
}
