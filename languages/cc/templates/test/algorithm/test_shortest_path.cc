#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "data-structure/graph.cc"
#include "algorithm/shortest_path.cc"
#include "../graph_utils.h"

LABELED_GRAPH_TEST(DijkstraTest);
LABELED_GRAPH_TEST(BellmanFordTest);
LABELED_GRAPH_TEST(WarshallFloydTest);

using namespace graph;

TYPED_TEST(g_DijkstraTest, SimpleTest) {
    /*
     * 0 -(5)----------> 1
     *   -(1)-> 2 -(2)->
     */
    std::vector<std::tuple<size_t, size_t, int>> edges;
    edges.push_back({0, 1, 5});
    edges.push_back({0, 2, 1});
    edges.push_back({2, 1, 2});

    auto g = this->mkGraph(edges);
    auto ans = dijkstra(g, 0);

    using std::make_optional;
    using optional = std::optional<size_t>;
    using C = CostWithPreviousVertex<typename decltype(g)::EdgeLabel>;
    EXPECT_EQ(3, ans.size());
    EXPECT_EQ(C(0, optional()), ans[0]);
    EXPECT_EQ(C(3, make_optional(2)), ans[1]);
    EXPECT_EQ(C(1, make_optional(0)), ans[2]);
}

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

    using std::make_optional;
    using optional = std::optional<size_t>;
    using C = CostWithPreviousVertex<typename decltype(g)::EdgeLabel>;
    EXPECT_EQ(3, ans.size());
    EXPECT_EQ(C(0, optional()), ans[0].value());
    EXPECT_EQ(C(3, make_optional(2)), ans[1].value());
    EXPECT_EQ(C(1, make_optional(0)), ans[2].value());
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

    using std::make_optional;
    using optional = std::optional<size_t>;
    using C = CostWithPreviousVertex<typename decltype(g)::EdgeLabel>;
    EXPECT_EQ(3, ans.size());
    EXPECT_EQ(C(0, optional()), ans[0].value());
    EXPECT_EQ(C(-10, make_optional(2)), ans[1].value());
    EXPECT_EQ(C(10, make_optional(0)), ans[2].value());
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
    EXPECT_EQ(0, ans[0].value().cost);
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
    using C = CostWithPreviousVertex<typename decltype(g)::EdgeLabel>;
    using std::optional;
    using std::make_optional;
    using std::vector;
    auto ans = warshall_floyd(g);
    EXPECT_EQ(3, ans.size());
    EXPECT_EQ(vector<C>({C(0, optional<size_t>()), C(3, make_optional(2)), C(1, make_optional(0))}),
              ans[0]);
    EXPECT_EQ(vector<C>({C(3, make_optional(2)), C(0, optional<size_t>()), C(2, make_optional(1))}),
              ans[1]);
    EXPECT_EQ(vector<C>({C(1, make_optional(2)), C(2, make_optional(2)), C(0, optional<size_t>())}),
              ans[2]);
}
