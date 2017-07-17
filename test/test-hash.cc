#include <gtest/gtest.h>
#include "common.h"

TEST(HashTest, PairHashTest) {
    auto p1 = make_pair(0, 1);
    auto p2 = make_pair(0, 1);
    auto p3 = make_pair(1, 1);

    auto h = hash<decltype(p1)>();
    EXPECT_EQ(h(p1), h(p2));
    EXPECT_NE(h(p1), h(p3));
}

TEST(HashTest, TupleHashTest) {
    auto t1 = make_tuple(0, 1, 2);
    auto t2 = make_tuple(0, 1, 2);
    auto t3 = make_tuple(0, 1, 4);

    auto h = hash<decltype(t1)>();
    EXPECT_EQ(h(t1), h(t2));
    EXPECT_NE(h(t1), h(t3));
}