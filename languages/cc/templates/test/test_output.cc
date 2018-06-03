#include <gtest/gtest.h>
#include "common.cc"

TEST(OutputTest, OutputValueTest) {
    testing::internal::CaptureStderr();
    dump(10);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("10\n"), output);
}
TEST(OutputTest, OutputMultipleValue) {
    testing::internal::CaptureStderr();
    dump(0, 'f');
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("0, f\n"), output);
}

TEST(OnelineOutputTest, OutputPairTest) {
    auto p = make_pair(0, 1);
    testing::internal::CaptureStderr();
    dump(p);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("{0, 1}\n"), output);
}
TEST(OnelineOutputTest, OutputTupleTest) {
    auto t = make_tuple(0, 1, 'f', make_pair(0, 1));
    testing::internal::CaptureStderr();
    dump(t);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("{0, 1, f, {0, 1}}\n"), output);
}
TEST(OnelineOutputTest, OutputBoolTest) {
    std::cerr << std::boolalpha;
    testing::internal::CaptureStderr();
    dump(true);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("true\n"), output);
}
TEST(OnelineOutputTest, OutputVectorTest) {
    auto v = Vector<int>({0, 1, 2});
    testing::internal::CaptureStderr();
    dump(v);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("[0, 1, 2]\n"), output);
}
TEST(OnelineOutputTest, OutputSetTest) {
    OrderedSet<int> s1 = {0, 1, 2};
    testing::internal::CaptureStderr();
    dump(s1);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("{0, 1, 2}\n"), output);

    HashSet<int> s2 = {0, 1, 2};
    testing::internal::CaptureStderr();
    dump(s2);
    output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("{2, 1, 0}\n"), output);
}
TEST(OnelineOutputTest, OutputMapTest) {
    OrderedMap<int, int> s1 = {{0, 0}, {1, -1}, {2, -2}};
    testing::internal::CaptureStderr();
    dump(s1);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("{0: 0, 1: -1, 2: -2}\n"), output);

    HashMap<int, int> s2 = {{0, 0}, {1, -1}, {2, -2}};
    testing::internal::CaptureStderr();
    dump(s2);
    output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("{2: -2, 1: -1, 0: 0}\n"), output);
}

TEST(MultilineOutputTest, OutputSetTest) {
    OrderedSet<int> s = {0, 1, 2};
    testing::internal::CaptureStderr();
    dump(as_set(s));
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("---\n0\n1\n2\n---\n"), output);
}
TEST(MultilineOutputTest, OutputMapTest) {
    auto v = Vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    testing::internal::CaptureStderr();
    dump(as_map(v));
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("---\n0 : 0\n1 : 1\n2 : 2\n3 : 3\n4 : 4\n5 : 5\n6 : 6\n7 : 7\n8 : 8\n9 : 9\n10: 10\n---\n"), output);

    OrderedMap<string, int> s = {{"x", 0}, {"xxx", -1}, {"y", -2}};
    testing::internal::CaptureStderr();
    dump(as_map(s));
    output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("---\nx  : 0\nxxx: -1\ny  : -2\n---\n"), output);
}
TEST(MultilineOutputTest, OutputTableTest) {
    auto v = Matrix<int, 2>({{0, 0, 0}, {10, 1}, {2, 200}});
    testing::internal::CaptureStderr();
    dump(as_table(v));
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("---\n   0 , 1  , 2\n------\n0| 0 , 0  , 0\n1| 10, 1  ,  \n2| 2 , 200,  \n---\n"), output);

    auto t = OrderedMap<std::string, OrderedMap<std::string, i64>>({{"x", {{"y", 1}}}, {"xx", {{"yy", 1}, {"y", 20}}}});
    testing::internal::CaptureStderr();
    dump(as_table(t));
    output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("---\n    y , yy\n------\nx | 1 ,   \nxx| 20, 1 \n---\n"), output);
}
