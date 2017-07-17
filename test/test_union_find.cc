#include <gtest/gtest.h>
#include <string>
#include "common.cc"
#include "data-structure/union_find.cc"

TEST(UnionFindTest, ConstructorTest) {
    union_find<string> f({"x", "y", "s", "t"});
    EXPECT_TRUE(f.is_same("x", "x"));
    EXPECT_FALSE(f.is_same("x", "y"));
    EXPECT_FALSE(f.is_same("x", "s"));
    EXPECT_FALSE(f.is_same("x", "t"));
}
TEST(UnionFindTest, MergeTest) {
    union_find<string> f({"x", "y", "s", "t"});

    f.merge("x", "y");
    f.merge("s", "t");
    EXPECT_TRUE(f.is_same("x", "x"));
    EXPECT_TRUE(f.is_same("x", "y"));
    EXPECT_FALSE(f.is_same("x", "s"));

    f.merge("y", "t");
    EXPECT_TRUE(f.is_same("x", "s"));
}
TEST(UnionFindTest, TemplateSpecializationTest) {
    union_find<size_t> f(4);
    EXPECT_TRUE(f.is_same(0, 0));
    EXPECT_FALSE(f.is_same(0, 1));
    EXPECT_FALSE(f.is_same(0, 3));

    f.merge(0, 1);
    EXPECT_TRUE(f.is_same(0, 0));
    EXPECT_TRUE(f.is_same(0, 1));
    EXPECT_FALSE(f.is_same(0, 3));
}