#include <gtest/gtest.h>
#include "common.cc"
#include "util/math.cc"

TEST(MathTest, PowTest) {
    EXPECT_EQ(1, math::pow(2, 0));
    EXPECT_EQ(2, math::pow(2, 1));
    EXPECT_EQ(4, math::pow(2, 2));
    EXPECT_EQ(8, math::pow(2, 3));
}
