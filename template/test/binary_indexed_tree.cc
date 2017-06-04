TEST(BinaryIndexedTreeTest, ConstructorTest) {
    BIT<int> b({0, 1, 2});

    EXPECT_EQ(3, b.size());
}
TEST(BinaryIndexedTreeTest, SimpleSumTest) {
    BIT<int> b({1, 3, 2});
    EXPECT_EQ(0, b.sum(0));
    EXPECT_EQ(1, b.sum(1));
    EXPECT_EQ(4, b.sum(2));
    EXPECT_EQ(6, b.sum(3));
}
TEST(BinaryIndexedTreeTest, SumTest) {
    BIT<int> b({1, 3, 2});
    EXPECT_EQ(1, b.sum(0, 1));
    EXPECT_EQ(5, b.sum(1, 3));
    EXPECT_EQ(6, b.sum(0, 3));
}
TEST(BinaryIndexedTreeTest, AtTest) {
    BIT<int> b({1, 3, 2});
    EXPECT_EQ(1, b.at(0));
    EXPECT_EQ(3, b.at(1));
    EXPECT_EQ(2, b.at(2));
}
TEST(BinaryIndexedTreeTest, AddTest) {
    BIT<int> b({1, 3, 2});

    b.add(1, 3);

    EXPECT_EQ(1, b.at(0));
    EXPECT_EQ(6, b.at(1));
    EXPECT_EQ(2, b.at(2));
}
