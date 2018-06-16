#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "util/math.cc"
#include "util/mod.cc"

using namespace mod_integer;

TEST(ModTest, ConstructorTest) {
    EXPECT_EQ(0, ModInteger<>().get());
    EXPECT_EQ(MOD - 1, ModInteger<>(-1).get());
    EXPECT_EQ(1, ModInteger<>(MOD + 1).get());
}

TEST(ModTest, InverseTest) {
    ModInteger<> x(10);
    auto y = x.inverse();
    EXPECT_EQ(1, x * y);
}

TEST(ModTest, PlusTest) {
    EXPECT_EQ(0, ModInteger<>(MOD) + MOD);
}
TEST(ModTest, MinusTest) {
    EXPECT_EQ(1, ModInteger<>(1) - MOD);
}
TEST(ModTest, Multiplytest) {
    EXPECT_EQ(ModInteger<>(-2), ModInteger<>(MOD-1) * 2);
}
TEST(ModTest, DivideTest) {
    EXPECT_EQ(ModInteger<>(2), ModInteger<>(20) / 10);
}

TEST(ModTest, CombinationTest) {
    EXPECT_EQ(ModInteger<>(4), combination(ModInteger<>(4), 1));
}

TEST(ModTest, FactTableTest) {
    auto table = fact_table<MOD>(3);
    EXPECT_EQ(4, table.size());

    EXPECT_EQ(1, table[0].value);
    EXPECT_EQ(1, table[0].inverse);

    EXPECT_EQ(1, table[1].value);
    EXPECT_EQ(1, table[1].inverse);

    EXPECT_EQ(2, table[2].value);
    EXPECT_EQ(ModInteger<>(2).inverse(), table[2].inverse);

    EXPECT_EQ(6, table[3].value);
    EXPECT_EQ(ModInteger<>(6).inverse(), table[3].inverse);
}

TEST(ModTest, OutputTest) {
    ModInteger<> x(1);
    testing::internal::CaptureStderr();
    dump(x);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("1(mod 1000000007)\n"), output);
}
