#include <gtest/gtest.h>
#include <sstream>
#include "common.cc"

TEST(InputTest, ReadPairTest) {
    stringbuf buf;
    buf.sputn("0 10", 4);
    auto prev = cin.rdbuf(&buf);
    auto pair = read<int, int>();

    cin.rdbuf(prev);

    EXPECT_EQ(0, pair.first);
    EXPECT_EQ(10, pair.second);
}

TEST(InputTest, ReadTupleTest) {
    stringbuf buf;
    buf.sputn("0 10 f", 8);
    auto prev = cin.rdbuf(&buf);
    auto tuple = read<int, int, char>();

    cin.rdbuf(prev);

    EXPECT_EQ(0, get<0>(tuple));
    EXPECT_EQ(10, get<1>(tuple));
    EXPECT_EQ('f', get<2>(tuple));
}

TEST(InputTest, ReadVectorTest) {
    stringbuf buf;
    buf.sputn("0 10", 4);
    auto prev = cin.rdbuf(&buf);
    auto vector = read<int>(2);

    cin.rdbuf(prev);

    EXPECT_EQ(V<int>({0, 10}), vector);
}