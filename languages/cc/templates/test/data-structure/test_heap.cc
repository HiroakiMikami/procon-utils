#define MAIN

#include <gtest/gtest.h>
#include <numeric>
#include "common.cc"
#include "data-structure/heap.cc"

TEST(HeapTest, ConstructorTest) {
    Heap<int> h;
    h.push(0);
    h.push(1);
    h.push(2);

    EXPECT_EQ(3, h.size());
    EXPECT_EQ(2, h.top());
}

TEST(HeapTest, CompareTest) {
    Heap<int, std::greater<int>> h;
    h.push(0);
    h.push(1);
    h.push(2);
    EXPECT_EQ(0, h.top());
}

TEST(HeapTest, PushTest) {
    Heap<int> h;

    h.push(10);
    EXPECT_EQ(1, h.size());
    EXPECT_EQ(10, h.top());

    h.push(20);
    EXPECT_EQ(2, h.size());
    EXPECT_EQ(20, h.top());

    auto sum = std::accumulate(h.begin(), h.end(), 0);
    EXPECT_EQ(30, sum);
}

TEST(HeapTest, PopTest) {
    Heap<int> h;

    h.push(10);
    h.push(20);

    EXPECT_EQ(20, h.pop());

    EXPECT_EQ(1, h.size());
    EXPECT_EQ(10, h.top());
}