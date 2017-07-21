#include <gtest/gtest.h>
#include "common.cc"
#include "util/tag.cc"

TEST(TagTest, MakeTagsTest) {
    V<string> data = {"0", "1", "2"};

    auto x = make_tags(data);
    EXPECT_EQ("0", x[0]);
    EXPECT_EQ("1", x[1]);
    EXPECT_EQ("2", x[2]);
    EXPECT_EQ(0, x["0"]);
    EXPECT_EQ(1, x["1"]);
    EXPECT_EQ(2, x["2"]);
}
