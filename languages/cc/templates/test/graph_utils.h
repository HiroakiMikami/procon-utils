#pragma once

#ifndef MAIN
#include "common.cc"
#include "graph.cc"
#endif

template <class Graph>
struct AbstractGraphTest : public ::testing::Test {
    Graph mkGraph(const Vector<pair<size_t, size_t>> &edges) {
        size_t vertex_num = 0;
        EACH (e, edges) {
            vertex_num = max(vertex_num, max(e.first + 1, e.second + 1));
        }

        Graph g(vertex_num);
        EACH (e, edges) {
            g.add_edge({e.first, e.second});
        }
        return g;
    }
};

template <class Graph>
struct AbstractLabeledGraphTest : public ::testing::Test {
    template <class Edge>
    Graph mkGraph(const Vector<tuple<size_t, size_t, Edge>> &edges) {
        size_t vertex_num = 0;
        EACH (e, edges) {
            vertex_num = max(vertex_num, max(get<0>(e) + 1, get<1>(e) + 1));
        }

        Graph g(vertex_num);
        EACH (e, edges) {
            g.add_edge(e);
        }
        return g;
    }
};

typedef ::testing::Types<SimpleAdjacencyList, SimpleAdjacencyMatrix> Graphs;
typedef ::testing::Types<WeightedAdjacencyList> LabeledGraphs;
//typedef ::testing::Types<WeightedAdjacencyList, WeightedAdjacencyMatrix> LabeledGraphs;

#define GRAPH_TEST(name) template <class Graph> struct g_##name : public AbstractGraphTest<Graph> {}; \
                         TYPED_TEST_CASE(g_##name, Graphs)
#define LABELED_GRAPH_TEST(name) template <class Graph> struct g_##name : public AbstractLabeledGraphTest<Graph> {}; \
                         TYPED_TEST_CASE(g_##name, LabeledGraphs)
