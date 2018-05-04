#include <gtest/gtest.h>
#include "common.cc"

TEST(OutputTest, OutputValueTest) {
    testing::internal::CaptureStderr();
    dump(10);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("10\n"), output);
}
TEST(OutputTest, OutputPairTest) {
    auto p = make_pair(0, 1);
    testing::internal::CaptureStderr();
    dump(p);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("{0, 1}\n"), output);
}
TEST(OutputTest, OutputTupleTest) {
    auto t = make_tuple(0, 1, 'f');
    testing::internal::CaptureStderr();
    dump(t);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("{0, 1, f}\n"), output);
}
TEST(OutputTest, OutputMultipleValue) {
    testing::internal::CaptureStderr();
    dump(0, 'f');
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("0, f\n"), output);
}

TEST(OutputTest, OutputVectorTest) {
    auto v = Vector<int>({0, 1, 2});
    testing::internal::CaptureStderr();
    dump_matrix<int>(v);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("0 1 2 \n"), output);
}
TEST(OutputTest, OutputMatrixTest) {
    auto v = Matrix<int, 2>({{0, 0}, {1, 1}, {2, 2}});
    testing::internal::CaptureStderr();
    dump_matrix<int>(v);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("0 0 \n1 1 \n2 2 \n"), output);
}

TEST(OutputTest, OutputSetTest) {
    OrderedSet<int> s = {0, 1, 2};
    testing::internal::CaptureStderr();
    dump_set(s);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("0\n1\n2\n"), output);
}
TEST(OutputTest, OutputSeqTest) {
    Vector<int> s = {0, 1, 2};
    testing::internal::CaptureStderr();
    dump_seq(s);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("0, 0\n1, 1\n2, 2\n"), output);
}

TEST(OutputTest, OutputMapTest) {
    OrderedMap<int, int> s = {{0, 0}, {1, -1}, {2, -2}};
    testing::internal::CaptureStderr();
    dump_map(s);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("0\t:0\n1\t:-1\n2\t:-2\n"), output);
}
