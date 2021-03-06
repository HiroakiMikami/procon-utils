#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "util/math.cc"

TEST(MathTest, ModTest) {
    EXPECT_EQ(0, mod(1, 1));
    EXPECT_EQ(2, mod(5, 3));
    EXPECT_EQ(3, mod(-17, 5));
}

TEST(MathTest, PowTest) {
    EXPECT_EQ(1, pow(2, 0));
    EXPECT_EQ(2, pow(2, 1));
    EXPECT_EQ(4, pow(2, 2));
    EXPECT_EQ(8, pow(2, 3));
}

TEST(MathTest, GcdTest) {
    EXPECT_EQ(1, gcd(1, 1));
    EXPECT_EQ(2, gcd(2, 4));
    EXPECT_EQ(2, gcd(4, 2));
    EXPECT_EQ(2, gcd(2, 0));
    EXPECT_EQ(2, gcd(0, 2));
    EXPECT_EQ(3, gcd(6, 15));
}

TEST(MathTest, GcdCtrTest) {
    EXPECT_EQ(-1, gcd_ctr({}));
    EXPECT_EQ(1, gcd_ctr({1, 1}));
    EXPECT_EQ(2, gcd_ctr({2}));
    EXPECT_EQ(1, gcd_ctr({6, 15, 2}));
}

TEST(MathTest, ExtGcdTest) {
    EXPECT_EQ(make_tuple(1, 0, 1), ext_gcd(1, 0));
    EXPECT_EQ(make_tuple(0, 1, 1), ext_gcd(0, 1));
    EXPECT_EQ(make_tuple(1, 0, 2), ext_gcd(2, 4));
}

TEST(MathTest, LcmTest) {
    EXPECT_EQ(2, lcm(1, 2));
    EXPECT_EQ(4, lcm(2, 4));
    EXPECT_EQ(12, lcm(6, 4));
}

TEST(MathTest, LcmCtrTest) {
    EXPECT_EQ(-1, lcm_ctr({}));
    EXPECT_EQ(2, lcm_ctr({2}));
    EXPECT_EQ(4, lcm_ctr({2, 4}));
    EXPECT_EQ(60, lcm_ctr({15, 4, 12}));
}

TEST(MathTest, CombinationTest) {
    EXPECT_EQ(1, combination(4, 0));
    EXPECT_EQ(4, combination(4, 1));
    EXPECT_EQ(6, combination(4, 2));
    EXPECT_EQ(4, combination(4, 3));
    EXPECT_EQ(1, combination(4, 4));

    // TODO overflow test
}

TEST(MathTest, IsPrimeTest) {
    EXPECT_TRUE(is_prime(2));
    EXPECT_FALSE(is_prime(4));

    EXPECT_FALSE(is_prime(1));
}
TEST(MathTest, DivisorTest) {
    EXPECT_EQ(Vector<i64>({2, 6, 3, 4}), divisor(12));
}
TEST(MathTest, PrimeFactorTest) {
    auto expected = HashMap<i64, size_t>({{2, 2}, {3, 1}});
    EXPECT_EQ(expected, prime_factor(12));
}
TEST(MathTest, SieveTest) {
    auto res = sieve(15);
    EXPECT_EQ(Vector<i64>({2, 3, 5, 7, 11, 13}), res.first);
    EXPECT_EQ(
            Vector<bool>({false, false, true, true, false, true,
                           false, true, false, false, false,
                           true, false, true, false, false}),
            res.second
    );
}

TEST(MathTest, ChineseRemTest) {
    auto res = chinese_rem(2, 3, 3, 5);
    EXPECT_TRUE(res);
    EXPECT_EQ(res.value().first, 8);
    EXPECT_EQ(res.value().second, 15);

    EXPECT_FALSE(chinese_rem(2, 4, 4, 8));
}


TEST(MathTest, ChineseRemCtrTest) {
    auto res = chinese_rem_ctr(Vector<i64>{2, 3}, Vector<i64>{3, 5});
    EXPECT_TRUE(res);
    EXPECT_EQ(res.value().first, 8);
    EXPECT_EQ(res.value().second, 15);

    EXPECT_FALSE(chinese_rem_ctr(Vector<i64>{2, 4}, Vector<i64>{4, 8}));
}
