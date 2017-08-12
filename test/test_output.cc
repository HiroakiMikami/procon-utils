#include <gtest/gtest.h>
#include "common.cc"

using namespace debug;

TEST(OutputTest, OutputPairTest) {
    auto p = make_pair(0, 1);
    testing::internal::CaptureStdout();
    cout << p << flush;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(string("{0, 1}"), output);
}

TEST(OutputTest, OutputTupleTest) {
    auto t = make_tuple(0, 1, 'f');
    testing::internal::CaptureStdout();
    cout << t << flush;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(string("{0, 1, f}"), output);
}

TEST(OutputTest, OutputVectorTest) {
    auto v = V<int>({0, 1, 2});
    testing::internal::CaptureStdout();
    cout << v << flush;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(string("[0,\t1,\t2,\t\b\b]"), output);
}

TEST(OutputTest, OutputContainerTest) {
    auto v = V<int>({0, 1, 2});
    testing::internal::CaptureStdout();
    cout << container(CTR(v)) << flush;
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(string("[0,\t1,\t2,\t\b\b]"), output);
}
