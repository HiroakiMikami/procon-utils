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
            g.add_edge({e.first, e.second});
        }
        return g;
    }
};

template <class Graph>
struct LabeledGraphTest : public ::testing::Test {
    template <class Edge>
    Graph mkGraph(const V<tuple<size_t, size_t, Edge>> &edges) {
        size_t vertex_num = 0;
        FORE (e, edges) {
            vertex_num = max(vertex_num, max(get<0>(e) + 1, get<1>(e) + 1));
        }

        Graph g(vertex_num);
        FORE (e, edges) {
            g.add_edge(e);
        }
        return g;
    }
};

typedef ::testing::Types<SimpleAdjacencyList, SimpleAdjacencyMatrix> Graphs;
TYPED_TEST_CASE(GraphTest, Graphs);

TYPED_TEST(GraphTest, ConstructorTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});

    EXPECT_EQ(3, g.vertices_size());
    int cnt = 0;
    g.edges([&](const auto& edge) {
        ++cnt;
        return false;
    });
    EXPECT_EQ(3, cnt);
    EXPECT_TRUE(g.has_edge(0, 1));
    EXPECT_FALSE(g.has_edge(1, 0));
    cnt = 0;
    g.outgoings(1, [&](const auto &edge) {
        EXPECT_EQ(2, get<1>(edge));
        ++cnt;
        return false;
    });
    EXPECT_EQ(1, cnt);
}
TYPED_TEST(GraphTest, IterateEdgesTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});

    int cnt = 0;
    g.edges(0, 1, [&](const auto &edge) {
        EXPECT_EQ(0, get<0>(edge));
        EXPECT_EQ(1, get<1>(edge));
        ++cnt;
        return false;
    });
    EXPECT_EQ(1, cnt);
}
TYPED_TEST(GraphTest, BreakAtMiddleOfEdgesTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});

    int cnt = 0;
    g.edges([&](const auto &edge) {
        ++cnt;
        return true;
    });
    EXPECT_EQ(1, cnt);

    cnt = 0;
    g.edges(0, 1, [&](const auto &edge) {
        ++cnt;
        return true;
    });
    EXPECT_EQ(1, cnt);
}
TYPED_TEST(GraphTest, BreakAtMiddleOfOutgoingsTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});

    int cnt = 0;
    g.outgoings(0, [&](const auto &edge) {
        EXPECT_EQ(1, get<1>(edge));
        ++cnt;
        return true;
    });
    EXPECT_EQ(1, cnt);
}
TYPED_TEST(GraphTest, RemoveEdgeTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});
    EXPECT_TRUE(g.has_edge(0, 1));
    g.remove_edge({0, 1});
    EXPECT_FALSE(g.has_edge(0, 1));

    EXPECT_TRUE(g.has_edge(1, 2));
    g.remove_edge(1, 2);
    EXPECT_FALSE(g.has_edge(1, 2));
}
TYPED_TEST(GraphTest, RemoveVertexTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});
    EXPECT_EQ(3, g.vertices_size());
    g.remove_vertex(2);
    int cnt = 0;
    g.outgoings(2, [&](const auto &edge) {
        ++cnt;
        return false;
    });
    EXPECT_EQ(0, cnt);
}
TYPED_TEST(GraphTest, ToUndirectedTest) {
    auto g = this->mkGraph({{0, 1}, {0, 2}, {1, 2}});
    g.to_undirected();

    int cnt = 0;
    g.edges([&](const auto &edge) {
        ++cnt;
        return false;
    });
    EXPECT_EQ(6, cnt);
    EXPECT_TRUE(g.has_edge(1, 0));
}

typedef ::testing::Types<WeightedAdjacencyList, WeightedAdjacencyMatrix> LabeledGraphs;
TYPED_TEST_CASE(LabeledGraphTest, LabeledGraphs);

TYPED_TEST(LabeledGraphTest, ConstructorTest) {
    auto edges = V<tuple<size_t, size_t, i64>>();
    edges.push_back({0, 1, 10});
    edges.push_back({0, 2, 20});
    edges.push_back({1, 2, 30});
    auto g = this->mkGraph(edges);

    EXPECT_EQ(3, g.vertices_size());
    int cnt = 0;
    g.edges([&](const auto &edge) {
        ++cnt;
        return false;
    });
    EXPECT_EQ(3, cnt);
    cnt = 0;
    g.edges(0, 1, [&](const auto &edge) {
        EXPECT_EQ(make_tuple(0, 1, 10), edge);
        ++cnt;
        return false;
    });
    EXPECT_EQ(1, cnt);
    cnt = 0;
    g.edges(1, 0, [&](const auto &edge) {
        ++cnt;
        return false;
    });
    EXPECT_EQ(0, cnt);
    cnt = 0;
    g.outgoings(1, [&](const auto &edge) {
        EXPECT_EQ(2, get<1>(edge));
        EXPECT_EQ(30, get<2>(edge));
        ++cnt;
        return false;
    });
    EXPECT_EQ(1, cnt);
}
TYPED_TEST(LabeledGraphTest, IterateEdgesTest) {
    auto edges = V<tuple<size_t, size_t, i64>>();
    edges.push_back({0, 1, 10});
    edges.push_back({0, 2, 20});
    edges.push_back({1, 2, 30});
    auto g = this->mkGraph(edges);

    int cnt = 0;
    g.edges(0, 1, [&](const auto &edge) {
        EXPECT_EQ(0, get<0>(edge));
        EXPECT_EQ(1, get<1>(edge));
        EXPECT_EQ(10, get<2>(edge));
        ++cnt;
        return false;
    });
    EXPECT_EQ(1, cnt);
}
TYPED_TEST(LabeledGraphTest, BreakAtMiddleOfEdgesTest) {
    auto edges = V<tuple<size_t, size_t, i64>>();
    edges.push_back({0, 1, 10});
    edges.push_back({0, 2, 20});
    edges.push_back({1, 2, 30});
    auto g = this->mkGraph(edges);

    int cnt = 0;
    g.edges([&](const auto &edge) {
        ++cnt;
        return true;
    });
    EXPECT_EQ(1, cnt);

    cnt = 0;
    g.edges(0, 1, [&](const auto &edge) {
        ++cnt;
        return true;
    });
    EXPECT_EQ(1, cnt);
}
TYPED_TEST(LabeledGraphTest, BreakAtMiddleOfOutgoingsTest) {
    auto edges = V<tuple<size_t, size_t, i64>>();
    edges.push_back({0, 1, 10});
    edges.push_back({0, 2, 20});
    edges.push_back({1, 2, 30});
    auto g = this->mkGraph(edges);

    int cnt = 0;
    g.outgoings(0, [&](const auto &edge) {
        EXPECT_EQ(1, get<1>(edge));
        EXPECT_EQ(10, get<2>(edge));
        ++cnt;
        return true;
    });
    EXPECT_EQ(1, cnt);
}
TYPED_TEST(LabeledGraphTest, RemoveEdgeTest) {
    auto edges = V<tuple<size_t, size_t, i64>>();
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
TYPED_TEST(LabeledGraphTest, ToUndirectedTest) {
    auto edges = V<tuple<size_t, size_t, i64>>();
    edges.push_back({0, 1, 10});
    edges.push_back({0, 2, 20});
    edges.push_back({1, 2, 30});
    auto g = this->mkGraph(edges);
    g.to_undirected();

    int cnt = 0;
    g.edges([&](const auto &edge) {
        ++cnt;
        return false;
    });
    EXPECT_EQ(6, cnt);
    cnt = 0;
    g.edges(1, 0, [&](const auto &edge) {
        EXPECT_EQ(make_tuple(1, 0, 10L), edge);
        ++cnt;
        return false;
    });
    EXPECT_EQ(1, cnt);
}

