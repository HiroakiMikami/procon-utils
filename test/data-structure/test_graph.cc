#include <gtest/gtest.h>
#include "common.cc"
#include "data-structure/graph.cc"

using namespace graph;

TEST(EdgeSetTest, ConstructorTest) {
    auto g = EdgeSet({{0, 1}, {0, 2}, {1, 2}});

    EXPECT_EQ(3, g.vertices_size());
    EXPECT_EQ(3, g.edges().size());
    EXPECT_TRUE(g.has_edge(0, 1));
    EXPECT_FALSE(g.has_edge(1, 0));
    EXPECT_EQ(vector<size_t>({2}), g.outgoings(1));
    EXPECT_EQ(vector<size_t>(), g.incomings(0));
    EXPECT_EQ(vector<size_t>({2}), g.outgoings(0, [](auto e) { return e.second % 2 == 0; }));
}
TEST(EdgeSetTest, ToUndirectedTest) {
    auto g = EdgeSet({{0, 1}, {0, 2}, {1, 2}});
    g.to_undirected();

    EXPECT_EQ(6, g.edges().size());
    EXPECT_TRUE(g.has_edge(1, 0));
}
TEST(EdgeSetTest, AddEdgeTest) {
    auto g = EdgeSet({{0, 1}, {0, 2}, {1, 2}});
    EXPECT_FALSE(g.has_edge(1, 0));
    g.add_edge(1, 0);
    EXPECT_TRUE(g.has_edge(1, 0));
}
TEST(EdgeSetTest, RemoveEdgeTest) {
    auto g = EdgeSet({{0, 1}, {0, 2}, {1, 2}});
    EXPECT_TRUE(g.has_edge(0, 1));
    g.remove_edge(0, 1);
    EXPECT_FALSE(g.has_edge(0, 1));
}
TEST(EdgeSetTest, RemoveVertexTest) {
    auto g = EdgeSet({{0, 1}, {0, 2}, {1, 2}});
    EXPECT_EQ(3, g.vertices_size());
    g.remove_vertex(2);
    EXPECT_EQ(2, g.vertices_size());
}

/*
TEST(AdjacencyListTest, ToUndirectedTest) {

}
TEST(AdjacencyListTest, ConstructorTest) {
    // 0 <-> 1 -> 2
    auto g = AdjacencyList<void, true>({{1}, {0, 2}, {}});
    auto edges = V<pair<size_t, size_t>>({{0, 1}, {1, 0}, {1, 2}});

    EXPECT_EQ(3, g.vertices_size());
    EXPECT_EQ(edges, g.edges());
    EXPECT_EQ(true, g.has_edge(0, 1));
    EXPECT_EQ(false, g.has_edge(2, 1));
    EXPECT_EQ(V<size_t>({0, 2}), g.neighbors(1));
    EXPECT_EQ(V<size_t>({0}), g.neighbors(1, [](size_t n) { return n == 0; }));
}

TEST(AdjacencyListTest, UndirectedGraphConstructorTest) {

}

*/