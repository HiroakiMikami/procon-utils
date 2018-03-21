#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "data-structure/graph.cc"
#include "data-structure/union_find.cc"
#include "algorithm/minimum_spanning_tree.cc"
#include "../graph_utils.h"

LABELED_GRAPH_TEST(PrimTest);
LABELED_GRAPH_TEST(KruskalTest);

TYPED_TEST(g_PrimTest, SimpleTest) {
    using std::make_tuple;

    std::vector<std::tuple<size_t, size_t, int>> edges;
    edges.push_back({0, 1, 7});
    edges.push_back({0, 3, 5});
    edges.push_back({1, 2, 8});
    edges.push_back({1, 3, 9});
    edges.push_back({1, 4, 7});
    edges.push_back({2, 4, 5});
    edges.push_back({3, 4, 15});
    edges.push_back({3, 5, 6});
    edges.push_back({4, 5, 8});
    edges.push_back({4, 6, 9});
    edges.push_back({5, 6, 11});

    auto g = this->mkGraph(edges);
    g.to_undirected();
    auto ans = prim(g, 0);
    EXPECT_EQ(39, ans.first);
    auto expected = vector<Edge<typename decltype(g)::EdgeLabel>>{
            make_tuple(0, 3, 5), make_tuple(3, 5, 6),
            make_tuple(0, 1, 7), make_tuple(1, 4, 7),
            make_tuple(4, 2, 5), make_tuple(4, 6, 9)
    };
    EXPECT_EQ(expected, ans.second);
}
TYPED_TEST(g_KruskalTest, SimpleTest) {
    using std::make_tuple;

    std::vector<std::tuple<size_t, size_t, int>> edges;
    edges.push_back({0, 1, 7});
    edges.push_back({0, 3, 5});
    edges.push_back({1, 2, 8});
    edges.push_back({1, 3, 9});
    edges.push_back({1, 4, 7});
    edges.push_back({2, 4, 5});
    edges.push_back({3, 4, 15});
    edges.push_back({3, 5, 6});
    edges.push_back({4, 5, 8});
    edges.push_back({4, 6, 9});
    edges.push_back({5, 6, 11});

    auto g = this->mkGraph(edges);
    g.to_undirected();
    auto ans = kruskal(g);
    EXPECT_EQ(39, ans.first);
    auto expected = vector<Edge<typename decltype(g)::EdgeLabel>>{
            make_tuple(0, 3, 5), make_tuple(2, 4, 5),
            make_tuple(3, 5, 6),
            make_tuple(1, 4, 7), make_tuple(0, 1, 7),
            make_tuple(4, 6, 9)
    };
    EXPECT_EQ(expected, ans.second);
}
