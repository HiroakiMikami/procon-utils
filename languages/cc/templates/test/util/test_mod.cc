#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "util/math.cc"
#include "util/mod.cc"

using namespace mod;

TEST(ModTest, ConstructorTest) {
    EXPECT_EQ(0, Integer().get());
    EXPECT_EQ(MOD - 1, Integer(-1).get());
    EXPECT_EQ(1, Integer(MOD + 1).get());
}

TEST(ModTest, InverseTest) {
    Integer x(10);
    auto y = x.inverse();
    EXPECT_EQ(1, x * y);
}

TEST(ModTest, PlusTest) {
    EXPECT_EQ(0, Integer(MOD)+MOD);
}
TEST(ModTest, MinusTest) {
    EXPECT_EQ(1, Integer(1) - MOD);
}
TEST(ModTest, Multiplytest) {
    EXPECT_EQ(Integer(-2), Integer(MOD-1) * 2);
}
TEST(ModTest, DivideTest) {
    EXPECT_EQ(Integer(2), Integer(20) / 10);
}

TEST(ModTest, CombinationTest) {
    EXPECT_EQ(Integer(4), combination(Integer(4), 1));
}

TEST(ModTest, FactTableTest) {
    auto table = fact_table(3);
    EXPECT_EQ(4, table.size());

    EXPECT_EQ(1, table[0].value);
    EXPECT_EQ(1, table[0].inverse);

    EXPECT_EQ(1, table[1].value);
    EXPECT_EQ(1, table[1].inverse);

    EXPECT_EQ(2, table[2].value);
    EXPECT_EQ(Integer(2).inverse(), table[2].inverse);

    EXPECT_EQ(6, table[3].value);
    EXPECT_EQ(Integer(6).inverse(), table[3].inverse);
}
