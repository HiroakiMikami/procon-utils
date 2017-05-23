#include <numeric>

TEST(HeapTest, ConstructorTest) {
    heap<int> h({0, 1, 2});

    EXPECT_EQ(3, h.size());
    EXPECT_EQ(2, h.top());
}
TEST(HeapTest, CompareTest) {
    heap<int, greater<int>> h({0, 1, 2});
    EXPECT_EQ(0, h.top());
}
TEST(HeapTest, PushTest) {
    heap<int> h;

    h.push(10);
    EXPECT_EQ(1, h.size());
    EXPECT_EQ(10, h.top());

    h.push(20);
    EXPECT_EQ(2, h.size());
    EXPECT_EQ(20, h.top());

    auto sum = accumulate(h.begin(), h.end(), 0);
    EXPECT_EQ(30, sum);
}
TEST(HeapTest, PopTest) {
    heap<int> h;

    h.push(10);
    h.push(20);

    EXPECT_EQ(20, h.pop());

    EXPECT_EQ(1, h.size());
    EXPECT_EQ(10, h.top());
}