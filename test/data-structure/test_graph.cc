#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "data-structure/graph.cc"

template <class Graph>
struct GraphTest : public ::testing::Test {
    Graph mkGraph(const V<pair<size_t, size_t>> &edges) {
        size_t vertex_num = 0;
        FORE (e, edges) {
            vertex_num = max(vertex_num, max(e.first + 1, e.second + 1));
        }

        Graph g(vertex_num);
        FORE (e, edges) {
            g.add_edge(e.first, e.second);
        }
        return g;
    }
};

typedef ::testing::Types<AdjacencyList> Types;
TYPED_TEST_CASE(GraphTest, Types);

TYPED_TEST(GraphTest, ConstructorTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});

    EXPECT_EQ(3, g.vertices_size());
    EXPECT_EQ(3, g.edges().size());
    EXPECT_TRUE(g.has_edge(0, 1));
    EXPECT_FALSE(g.has_edge(1, 0));
    EXPECT_EQ(vector<size_t>({2}), g.outgoings(1));
    EXPECT_EQ(vector<size_t>(), g.incomings(0));
    EXPECT_EQ(vector<size_t>({2}), g.outgoings(0, [](auto e) { return e.second % 2 == 0; }));
}
TYPED_TEST(GraphTest, ToUndirectedTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});
    g.to_undirected();

    EXPECT_EQ(6, g.edges().size());
    EXPECT_TRUE(g.has_edge(1, 0));
}
TYPED_TEST(GraphTest, AddEdgeTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});
    EXPECT_FALSE(g.has_edge(1, 0));
    g.add_edge(1, 0);
    EXPECT_TRUE(g.has_edge(1, 0));
}
TYPED_TEST(GraphTest, RemoveEdgeTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});
    EXPECT_TRUE(g.has_edge(0, 1));
    g.remove_edge(0, 1);
    EXPECT_FALSE(g.has_edge(0, 1));
}
TYPED_TEST(GraphTest, RemoveVertexTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});
    EXPECT_EQ(3, g.vertices_size());
    g.remove_vertex(2);
    EXPECT_GE(3, g.vertices_size());
    EXPECT_LE(2, g.vertices_size());
}
