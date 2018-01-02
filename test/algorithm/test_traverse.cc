#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "data-structure/graph.cc"
#include "algorithm/traverse.cc"

template <class Graph>
struct DfsTest : public ::testing::Test {
    Graph mkGraph(const V<pair<size_t, size_t>> &edges) {
        size_t vertex_num = 0;
        FORE (e, edges) {
            vertex_num = max(vertex_num, max(e.first + 1, e.second + 1));
        }

        Graph g(vertex_num);
        FORE (e, edges) {
            g.add_edge({e.first, e.second});
        }
        return g;
    }
};

typedef ::testing::Types<SimpleAdjacencyList, SimpleAdjacencyMatrix> Graphs;
TYPED_TEST_CASE(DfsTest, Graphs);

TYPED_TEST(DfsTest, SimpleTest) {
    std::vector<size_t> nodes;
    dfs_with_duplicate_vertices(this->mkGraph({{0, 1}, {1, 2}, {2, 3}, {1, 3}}), {0}, [&](auto x) {
        nodes.push_back(get<1>(x));
        return false;
    }, [](const auto& edge) { return true; });

    EXPECT_EQ(4, nodes.size());
    EXPECT_EQ(1, nodes[0]);
    EXPECT_EQ(3, nodes[1]);
    EXPECT_EQ(2, nodes[2]);
    EXPECT_EQ(3, nodes[3]);

    nodes.clear();
    dfs(this->mkGraph({{0, 1}, {0, 3}, {1, 2}, {3, 4}}), {0}, [&](auto x) {
        nodes.push_back(get<1>(x));
        return false;
    }, [](const auto& edge) { return true; });

    EXPECT_EQ(4, nodes.size());
    EXPECT_TRUE(nodes[0] == 1 || nodes[0] == 3);
    EXPECT_EQ((nodes[0] == 1) ? 2 : 4, nodes[1]);
    EXPECT_TRUE(nodes[2] == 1 || nodes[2] == 3);
    EXPECT_EQ((nodes[2] == 1) ? 2 : 4, nodes[3]);
}
TYPED_TEST(DfsTest, BreakTest) {
    std::vector<size_t> nodes;
    dfs_with_duplicate_vertices(this->mkGraph({{0, 1}, {0, 2}, {1, 2}}), {0}, [&](auto x) {
        nodes.push_back(get<1>(x));
        return get<1>(x) == 1;
    }, [](const auto& edge) { return true; });

    EXPECT_EQ(1, nodes.back());
}
TYPED_TEST(DfsTest, IsAddedTest) {
    std::vector<size_t> nodes;
    dfs_with_duplicate_vertices(this->mkGraph({{0, 1}, {0, 2}, {1, 2}}), {0}, [&](auto x) {
        nodes.push_back(get<1>(x));
        return false;
    }, [](const auto& edge) { return get<1>(edge) != 2; });

    EXPECT_EQ(1, nodes.size());
    EXPECT_EQ(1, nodes[0]);
}
