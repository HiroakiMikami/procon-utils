#define MAIN

#include <gtest/gtest.h>
#include "common.cc"

TEST(IteratorUtilsTest, MappedIteratorTest) {
    auto s = OrderedSet<int>{1, 2, 3};
    int cnt = 0;
    for (auto x: iterator_map(s, [](const int &x) { return x * 2; })) {
        EXPECT_EQ((*s.begin() + cnt) * 2, x);
        cnt += 1;
    }
    EXPECT_EQ(3, cnt);
}
TEST(IteratorUtilsTest, FilteredIteratorTest) {
    auto s = Vector<int>{1, 2, 3, 4, 5, 6};
    int cnt = 0;
    for (auto x: iterator_filter(s, [](const int &x) { return x % 2 == 0; })) {
        EXPECT_EQ(x, s[cnt * 2 + 1]);
        cnt += 1;
    }
    EXPECT_EQ(3, cnt);
}
TEST(IteratorUtilsTest, FlattenedIteratorTest) {
    auto s = Matrix<int, 2>{{1}, {2}, {3}, {4}, {5}, {6}};
    int cnt = 0;
    for (auto x: iterator_flatten(s)) {
        EXPECT_EQ(x, s[cnt][0]);
        cnt += 1;
    }
    EXPECT_EQ(6, cnt);
}

TEST(IteratorUtilsTest, CombineMultipleIteratorHelperTest) {
    auto s = Matrix<int, 2>{{1}, {2}, {3}, {4}, {5}, {6}};
    int cnt = 0;
    for (auto x:
            iterator_map(
                    iterator_filter(iterator_flatten(s), [](auto x) { return x % 2 == 0; }),
                    [](auto x) { return x + 1; })){
        EXPECT_EQ(x, s[cnt*2+1][0]+1);
        cnt += 1;
    }
    EXPECT_EQ(3, cnt);
}

