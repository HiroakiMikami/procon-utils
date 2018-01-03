#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "data-structure/graph.cc"
#include "algorithm/shortest_path.cc"
#include "../graph_utils.h"

LABELED_GRAPH_TEST(BellmanFordTest);
LABELED_GRAPH_TEST(WarshallFloydTest);

TYPED_TEST(g_BellmanFordTest, SimpleTest) {
    /*
     * 0 -(5)----------> 1
     *   -(1)-> 2 -(2)->
     */
    std::vector<std::tuple<size_t, size_t, int>> edges;
    edges.push_back({0, 1, 5});
    edges.push_back({0, 2, 1});
    edges.push_back({2, 1, 2});

    auto g = this->mkGraph(edges);
    auto ans = bellman_ford(g, 0);
    EXPECT_EQ(3, ans.size());
    EXPECT_EQ(0, ans[0].value());
    EXPECT_EQ(3, ans[1].value());
    EXPECT_EQ(1, ans[2].value());
}
TYPED_TEST(g_BellmanFordTest, NegativeEdgeTest) {
    /*
     * 0 -(5)-------------> 1
     *   -(10)-> 2 -(-20)->
     */
    std::vector<std::tuple<size_t, size_t, int>> edges;
    edges.push_back({0, 1, 5});
    edges.push_back({0, 2, 10});
    edges.push_back({2, 1, -20});

    auto g = this->mkGraph(edges);
    auto ans = bellman_ford(g, 0);
    EXPECT_EQ(3, ans.size());
    EXPECT_EQ(0, ans[0].value());
    EXPECT_EQ(-10, ans[1].value());
    EXPECT_EQ(10, ans[2].value());
}
TYPED_TEST(g_BellmanFordTest, NegativeCycleTest) {
    /*
     * 0 -(5)--------------> 1
     *   -(10)-> 2 <-(-20)->
     */
    std::vector<std::tuple<size_t, size_t, int>> edges;
    edges.push_back({0, 1, 5});
    edges.push_back({0, 2, 10});
    edges.push_back({2, 1, -20});
    edges.push_back({1, 2, -20});

    auto g = this->mkGraph(edges);
    auto ans = bellman_ford(g, 0);
    EXPECT_EQ(3, ans.size());
    EXPECT_EQ(0, ans[0].value());
    EXPECT_FALSE(ans[1]);
    EXPECT_FALSE(ans[2]);
}

TYPED_TEST(g_WarshallFloydTest, SimpleTest) {
    /*
     * 0 <-(5)-----------> 1
     *   <-(1)-> 2 <-(2)->
     */
    std::vector<std::tuple<size_t, size_t, int>> edges;
    edges.push_back({0, 1, 5});
    edges.push_back({1, 0, 5});
    edges.push_back({0, 2, 1});
    edges.push_back({2, 0, 1});
    edges.push_back({2, 1, 2});
    edges.push_back({1, 2, 2});


    auto g = this->mkGraph(edges);
    auto ans = warshall_floyd(g);
    EXPECT_EQ(3, ans.size());
    EXPECT_EQ(std::vector<i64>({0, 3, 1}), ans[0]);
    EXPECT_EQ(std::vector<i64>({3, 0, 2}), ans[1]);
    EXPECT_EQ(std::vector<i64>({1, 2, 0}), ans[2]);
}
