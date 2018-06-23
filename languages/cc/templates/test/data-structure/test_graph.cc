#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "data-structure/graph.cc"
#include "../graph_utils.h"

GRAPH_TEST(GraphTest);
LABELED_GRAPH_TEST(LabeledGraphTest);

TYPED_TEST(g_GraphTest, ConstructorTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});

    EXPECT_EQ(3, g.vertices_size());
    auto edges = OrderedSet<tuple<size_t, size_t>>();
    EACH_V(elem, g.edges()) {
        edges.insert(elem);
    }
    EXPECT_EQ(3, edges.size());
    EXPECT_TRUE(edges.find(make_tuple(0, 1)) != edges.end());
    EXPECT_TRUE(edges.find(make_tuple(0, 2)) != edges.end());
    EXPECT_TRUE(edges.find(make_tuple(1, 2)) != edges.end());

    EXPECT_TRUE(g.has_edge(0, 1));
    EXPECT_FALSE(g.has_edge(1, 0));
    auto outgoings = OrderedSet<tuple<size_t, size_t>>();
    EACH_V(elem, g.outgoings(1)) {
        outgoings.insert(elem);
    }
    EXPECT_EQ(1, outgoings.size());
    EXPECT_TRUE(outgoings.find(make_tuple(1, 2)) != outgoings.end());
}
TYPED_TEST(g_GraphTest, IterateEdgesTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});

    int cnt = 0;
    EACH_V(edge, g.edges(0, 1)) {
        EXPECT_EQ(0, get<0>(edge));
        EXPECT_EQ(1, get<1>(edge));
        ++cnt;
    }
    EXPECT_EQ(1, cnt);
}
TYPED_TEST(g_GraphTest, RemoveEdgeTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});
    EXPECT_TRUE(g.has_edge(0, 1));
    g.remove_edge({0, 1});
    EXPECT_FALSE(g.has_edge(0, 1));

    EXPECT_TRUE(g.has_edge(1, 2));
    g.remove_edge(1, 2);
    EXPECT_FALSE(g.has_edge(1, 2));
}
TYPED_TEST(g_GraphTest, RemoveVertexTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});
    EXPECT_EQ(3, g.vertices_size());
    g.remove_vertex(2);
    int cnt = 0;
    EACH_V(edge, g.outgoings(2)) {
        ++cnt;
    }
    EXPECT_EQ(0, cnt);
}
TYPED_TEST(g_GraphTest, ToUndirectedTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});
    g.to_undirected();

    auto edges = OrderedSet<tuple<size_t, size_t>>();
    EACH_V (elem, g.edges()) {
        edges.insert(elem);
    }
    EXPECT_EQ(6, edges.size());
    EXPECT_TRUE(edges.find(make_tuple(0, 1)) != edges.end());
    EXPECT_TRUE(edges.find(make_tuple(1, 0)) != edges.end());
    EXPECT_TRUE(edges.find(make_tuple(0, 2)) != edges.end());
    EXPECT_TRUE(edges.find(make_tuple(2, 0)) != edges.end());
    EXPECT_TRUE(edges.find(make_tuple(1, 2)) != edges.end());
    EXPECT_TRUE(edges.find(make_tuple(2, 1)) != edges.end());

    EXPECT_TRUE(g.has_edge(1, 0));
}

TYPED_TEST(g_LabeledGraphTest, ConstructorTest) {
    auto edges = Vector<tuple<size_t, size_t, i64>>();
    edges.push_back({0, 1, 10});
    edges.push_back({0, 2, 20});
    edges.push_back({1, 2, 30});
    auto g = this->mkGraph(edges);

    EXPECT_EQ(3, g.vertices_size());

    int cnt = 0;
    auto e1 = g.edges();
    for (auto it: e1) {
        ++cnt;
    }
    EXPECT_EQ(3, cnt);

    auto e2 = g.edges(0, 1);
    cnt = 0;
    for (auto edge: e2) {
        EXPECT_EQ(make_tuple(0, 1, 10), edge);
        ++cnt;
    }
    EXPECT_EQ(1, cnt);

    auto e3 = g.edges(1, 0);
    cnt = 0;
    for (auto it: e3) {
        ++cnt;
    }
    EXPECT_EQ(0, cnt);

    auto e4 = g.outgoings(1);
    cnt = 0;
    for (auto edge: e4) {
        EXPECT_EQ(2, get<1>(edge));
        EXPECT_EQ(30, get<2>(edge));
        ++cnt;
    }
    EXPECT_EQ(1, cnt);
}
TYPED_TEST(g_LabeledGraphTest, IterateEdgesTest) {
    auto edges = Vector<tuple<size_t, size_t, i64>>();
    edges.push_back({0, 1, 10});
    edges.push_back({0, 2, 20});
    edges.push_back({1, 2, 30});
    auto g = this->mkGraph(edges);

    int cnt = 0;
    auto e1 = g.edges(0, 1);
    for (auto edge: e1) {
        EXPECT_EQ(0, get<0>(edge));
        EXPECT_EQ(1, get<1>(edge));
        EXPECT_EQ(10, get<2>(edge));
        ++cnt;
    }
    EXPECT_EQ(1, cnt);
}
TYPED_TEST(g_LabeledGraphTest, RemoveEdgeTest) {
    auto edges = Vector<tuple<size_t, size_t, i64>>();
    edges.push_back({0, 1, 10});
    edges.push_back({0, 2, 20});
    edges.push_back({1, 2, 30});
    auto g = this->mkGraph(edges);

    EXPECT_TRUE(g.has_edge(0, 1));
    g.remove_edge({0, 1, 20});
    EXPECT_TRUE(g.has_edge(0, 1));
    g.remove_edge({0, 1, 10});
    EXPECT_FALSE(g.has_edge(0, 1));

    EXPECT_TRUE(g.has_edge(1, 2));
    g.remove_edge(1, 2);
    EXPECT_FALSE(g.has_edge(1, 2));
}
TYPED_TEST(g_LabeledGraphTest, ToUndirectedTest) {
    auto edges = Vector<tuple<size_t, size_t, i64>>();
    edges.push_back({0, 1, 10});
    edges.push_back({0, 2, 20});
    edges.push_back({1, 2, 30});
    auto g = this->mkGraph(edges);
    g.to_undirected();

    int cnt = 0;
    auto e1 = g.edges();
    for (auto edge: e1) {
        ++cnt;
    }
    EXPECT_EQ(6, cnt);

    auto e2 = g.edges(1, 0);
    cnt = 0;
    for (auto edge: e2) {
        EXPECT_EQ(make_tuple(1, 0, 10L), edge);
        ++cnt;
    }
    EXPECT_EQ(1, cnt);
}

TYPED_TEST(g_GraphTest, OutputTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});
    testing::internal::CaptureStderr();
    dump(g);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("{[0, 1, 2], [{0, 1}, {0, 2}, {1, 2}]}\n"), output);
}

TYPED_TEST(g_LabeledGraphTest, OutputTest) {
    auto edges = Vector<tuple<size_t, size_t, i64>>();
    edges.push_back({0, 1, 10});
    edges.push_back({0, 2, 20});
    edges.push_back({1, 2, 30});
    auto g = this->mkGraph(edges);
    testing::internal::CaptureStderr();
    dump(g);
    auto output = testing::internal::GetCapturedStderr();
    EXPECT_EQ(string("{[0, 1, 2], [{0, 1, 10}, {0, 2, 20}, {1, 2, 30}]}\n"), output);
}
