#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "data-structure/graph.cc"
#include "algorithm/traverse.cc"
#include "../graph_utils.h"

GRAPH_TEST(DfsTest);
GRAPH_TEST(BfsTest);
LABELED_GRAPH_TEST(01BfsTest);

using namespace graph;

TYPED_TEST(g_DfsTest, SimpleTest) {
    std::vector<size_t> nodes;
    auto g1 = this->mkGraph({{0, 1}, {1, 2}, {2, 3}, {1, 3}});
    dfs(g1, {0}, [&](auto e __attribute__((unused)), auto x) {
        nodes.push_back(x);
        return false;
    }, filter_nothing(g1));

    EXPECT_EQ(5, nodes.size());
    EXPECT_EQ(0, nodes[0]);
    EXPECT_EQ(1, nodes[1]);
    EXPECT_EQ(3, nodes[2]);
    EXPECT_EQ(2, nodes[3]);
    EXPECT_EQ(3, nodes[4]);

    nodes.clear();
    auto g2 = this->mkGraph({{0, 1}, {0, 3}, {1, 2}, {3, 4}, {4, 2}});
    dfs(g2, {0}, [&](auto e __attribute__((unused)), auto x) {
        nodes.push_back(x);
        return false;
    }, filter_duplicated(g2));

    EXPECT_EQ(5, nodes.size());
    EXPECT_EQ(0, nodes[0]);
    EXPECT_EQ(3, nodes[1]);
    EXPECT_EQ(4, nodes[2]);
    EXPECT_EQ(2, nodes[3]);
    EXPECT_EQ(1, nodes[4]);
}
TYPED_TEST(g_DfsTest, BreakTest) {

    std::vector<size_t> nodes;
    auto g1 = this->mkGraph({{0, 1}, {1, 2}});
    dfs(g1, {0}, [&](auto e __attribute__((unused)), auto x) {
        nodes.push_back(x);
        return x == 1;
    }, filter_nothing(g1));

    EXPECT_EQ(2, nodes.size());
    EXPECT_EQ(0, nodes[0]);
    EXPECT_EQ(1, nodes[1]);
}
TYPED_TEST(g_DfsTest, FilterTest) {
    std::vector<size_t> nodes;
    auto g1 = this->mkGraph({{0, 1}, {1, 2}});
    dfs(g1, {0}, [&](auto e __attribute__((unused)), auto x) {
        nodes.push_back(x);
        return false;
    }, [](const auto& edge __attribute__((unused)), size_t v) { return v == 2; });

    EXPECT_EQ(2, nodes.size());
    EXPECT_EQ(0, nodes[0]);
    EXPECT_EQ(1, nodes[1]);
}

TYPED_TEST(g_BfsTest, SimpleTest) {
    std::vector<size_t> nodes;
    auto g1 = this->mkGraph({{0, 1}, {1, 2}, {2, 3}, {1, 3}});
    bfs(g1, {0}, [&](auto e __attribute__((unused)), auto x) {
        nodes.push_back(x);
        return false;
    }, filter_nothing(g1));

    EXPECT_EQ(5, nodes.size());
    EXPECT_EQ(0, nodes[0]);
    EXPECT_EQ(1, nodes[1]);
    EXPECT_EQ(2, nodes[2]);
    EXPECT_EQ(3, nodes[3]);
    EXPECT_EQ(3, nodes[4]);

    nodes.clear();
    auto g2 = this->mkGraph({{0, 1}, {0, 3}, {1, 2}, {3, 4}});
    bfs(g2, {0}, [&](auto e __attribute__((unused)), auto x) {
        nodes.push_back(x);
        return false;
    }, filter_duplicated(g2));

    EXPECT_EQ(5, nodes.size());
    EXPECT_EQ(0, nodes[0]);
    EXPECT_TRUE(nodes[1] == 1 || nodes[1] == 3);
    EXPECT_EQ((nodes[1] == 1) ? 3 : 1, nodes[2]);
    EXPECT_TRUE(nodes[3] == 2 || nodes[3] == 4);
    EXPECT_EQ((nodes[3] == 2) ? 4 : 2, nodes[4]);
}
TYPED_TEST(g_BfsTest, BreakTest) {
    std::vector<size_t> nodes;
    auto g1 = this->mkGraph({{0, 1}, {1, 2}, {1, 3}});
    bfs(g1, {0}, [&](auto e __attribute__((unused)), auto x) {
        nodes.push_back(x);
        return x == 1;
    }, filter_nothing(g1));

    EXPECT_EQ(2, nodes.size());
    EXPECT_EQ(0, nodes[0]);
    EXPECT_EQ(1, nodes[1]);
}
TYPED_TEST(g_BfsTest, FilterNotTest) {
    std::vector<size_t> nodes;
    auto g1 = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});
    bfs(g1, {0}, [&](auto e __attribute__((unused)), auto x) {
        nodes.push_back(x);
        return false;
    }, [](const auto& e __attribute__((unused)), size_t v) { return v == 2; });

    EXPECT_EQ(2, nodes.size());
    EXPECT_EQ(0, nodes[0]);
    EXPECT_EQ(1, nodes[1]);
}

TYPED_TEST(g_01BfsTest, SimpleTest) {
    std::vector<std::tuple<size_t, size_t, int>> edges;
    edges.push_back({0, 1, 1});
    edges.push_back({0, 2, 0});
    edges.push_back({1, 3, 0});
    edges.push_back({2, 4, 1});

    auto g = this->mkGraph(edges);
    std::vector<size_t> nodes;
    zero_one_bfs(g, {0}, [&](const auto &e __attribute__((unused)), size_t v) {
        nodes.push_back(v);
        return false;
    }, filter_duplicated(g));

    EXPECT_EQ(std::vector<size_t>({0, 2, 1, 3, 4}), nodes);
}
