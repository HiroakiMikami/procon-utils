#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "util/math.cc"

TEST(MathTest, PowTest) {
    EXPECT_EQ(1, math::pow(2, 0));
    EXPECT_EQ(2, math::pow(2, 1));
    EXPECT_EQ(4, math::pow(2, 2));
    EXPECT_EQ(8, math::pow(2, 3));
}

TEST(MathTest, GcdTest) {
    EXPECT_EQ(1, math::gcd(1, 1));
    EXPECT_EQ(2, math::gcd(2, 4));
    EXPECT_EQ(2, math::gcd(4, 2));
    EXPECT_EQ(2, math::gcd(2, 0));
    EXPECT_EQ(2, math::gcd(0, 2));
    EXPECT_EQ(3, math::gcd(6, 15));
}

TEST(MathTest, LeastCommonMultipleTest) {
    EXPECT_EQ(2, math::least_common_multiple(1, 2));
    EXPECT_EQ(4, math::least_common_multiple(2, 4));
    EXPECT_EQ(12, math::least_common_multiple(6, 4));
}

TEST(MathTest, CombinationTest) {
    EXPECT_EQ(1, math::combination(4, 0));
    EXPECT_EQ(4, math::combination(4, 1));
    EXPECT_EQ(6, math::combination(4, 2));
    EXPECT_EQ(4, math::combination(4, 3));
    EXPECT_EQ(1, math::combination(4, 4));

    // TODO overlow test
}