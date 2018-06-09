#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "util/math.cc"
#include "util/mod.cc"

using namespace mod;

TEST(ModTest, ConstructorTest) {
    EXPECT_EQ(0, Int().get());
    EXPECT_EQ(MOD - 1, Int(-1).get());
    EXPECT_EQ(1, Int(MOD + 1).get());
}

TEST(ModTest, InverseTest) {
    Int x(10);
    auto y = x.inverse();
    EXPECT_EQ(1, x * y);
}

TEST(ModTest, PlusTest) {
    EXPECT_EQ(0, Int(MOD)+MOD);
}
TEST(ModTest, MinusTest) {
    EXPECT_EQ(1, Int(1) - MOD);
}
TEST(ModTest, Multiplytest) {
    EXPECT_EQ(Int(-2), Int(MOD-1) * 2);
}
TEST(ModTest, DivideTest) {
    EXPECT_EQ(Int(2), Int(20) / 10);
}

TEST(ModTest, CombinationTest) {
    EXPECT_EQ(Int(4), combination(Int(4), 1));
}

TEST(ModTest, FactTableTest) {
    auto table = fact_table<mod::MOD>(3);
    EXPECT_EQ(4, table.size());

    EXPECT_EQ(1, table[0].value);
    EXPECT_EQ(1, table[0].inverse);

    EXPECT_EQ(1, table[1].value);
    EXPECT_EQ(1, table[1].inverse);

    EXPECT_EQ(2, table[2].value);
    EXPECT_EQ(Int(2).inverse(), table[2].inverse);

    EXPECT_EQ(6, table[3].value);
    EXPECT_EQ(Int(6).inverse(), table[3].inverse);
}

TEST(ModTest, OutputTest) {
    Int x(1);
    testing::internal::CaptureStderr();
    dump(x);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("1(mod 1000000007)\n"), output);
}
