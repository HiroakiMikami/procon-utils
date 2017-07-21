#include <gtest/gtest.h>
#include <string>
#include "common.cc"
#include "data-structure/union_find.cc"

TEST(UnionFindTest, ConstructorTest) {
    union_find f(4);
    EXPECT_TRUE(f.is_same(0, 0));
    EXPECT_FALSE(f.is_same(0, 1));
    EXPECT_FALSE(f.is_same(0, 2));
    EXPECT_FALSE(f.is_same(0, 3));
}
TEST(UnionFindTest, MergeTest) {
    union_find f(4);

    f.merge(0, 1);
    f.merge(2, 3);
    EXPECT_TRUE(f.is_same(0, 0));
    EXPECT_TRUE(f.is_same(0, 1));
    EXPECT_FALSE(f.is_same(0, 2));

    f.merge(1, 3);
    EXPECT_TRUE(f.is_same(0, 2));
}
