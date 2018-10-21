#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "data-structure/graph.cc"
#include "algorithm/flow.cc"
#include "../graph_utils.h"

LABELED_GRAPH_TEST(FordFulkersonTest);

using namespace graph;

TYPED_TEST(g_FordFulkersonTest, SimpleTest) {
    std::vector<std::tuple<size_t, size_t, int>> edges;
    edges.push_back({0, 1, 10});
    edges.push_back({0, 2, 2});
    edges.push_back({1, 2, 6});
    edges.push_back({1, 3, 6});
    edges.push_back({2, 4, 5});
    edges.push_back({3, 2, 3});
    edges.push_back({3, 4, 8});

    auto g = this->mkGraph(edges);
    EXPECT_EQ(11, ford_fulkerson(g, 0, 4));
}
