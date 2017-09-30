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

TEST(MathTest, GcdCtrTest) {
    EXPECT_EQ(-1, math::gcd_ctr({}));
    EXPECT_EQ(1, math::gcd_ctr({1, 1}));
    EXPECT_EQ(2, math::gcd_ctr({2}));
    EXPECT_EQ(1, math::gcd_ctr({6, 15, 2}));
}

TEST(MathTest, LcmTest) {
    EXPECT_EQ(2, math::lcm(1, 2));
    EXPECT_EQ(4, math::lcm(2, 4));
    EXPECT_EQ(12, math::lcm(6, 4));
}

TEST(MathTest, LcmCtrTest) {
    EXPECT_EQ(-1, math::lcm_ctr({}));
    EXPECT_EQ(2, math::lcm_ctr({2}));
    EXPECT_EQ(4, math::lcm_ctr({2, 4}));
    EXPECT_EQ(60, math::lcm_ctr({15, 4, 12}));
}

TEST(MathTest, CombinationTest) {
    EXPECT_EQ(1, math::combination(4, 0));
    EXPECT_EQ(4, math::combination(4, 1));
    EXPECT_EQ(6, math::combination(4, 2));
    EXPECT_EQ(4, math::combination(4, 3));
    EXPECT_EQ(1, math::combination(4, 4));

    // TODO overlow test
}

TEST(MathTest, IsPrimeTest) {
    EXPECT_TRUE(math::is_prime(2));
    EXPECT_FALSE(math::is_prime(4));

    EXPECT_FALSE(math::is_prime(1));
}
TEST(MathTest, DivisorTest) {
    EXPECT_EQ(V<i64>({2, 6, 3, 4}), math::divisor(12));
}
TEST(MathTest, PrimeFactorTest) {
    auto expected = unordered_map<i64, size_t>({{2, 2}, {3, 1}});
    EXPECT_EQ(expected, math::prime_factor(12));
}
TEST(MathTest, SieveTest) {
    auto res = math::sieve(15);
    EXPECT_EQ(V<i64>({2, 3, 5, 7, 11, 13}), res.first);
    EXPECT_EQ(
            V<bool>({false, false, true, true, false, true,
                     false, true, false, false, false,
                     true, false, true, false, false
                    }),
            res.second
    );
}