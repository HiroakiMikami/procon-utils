#include <limits>
#include <experimental/optional>

#ifndef MAIN
#include "common.cc"
#include "graph.cc"
#endif

template <class Graph>
std::vector<std::experimental::optional<i64>> bellman_ford(const Graph &g, size_t start) {
    auto max = std::numeric_limits<i64>::max();
    auto N = g.vertices_size();

    auto vertices = std::vector<std::experimental::optional<i64>>(N, std::experimental::make_optional(max));
    vertices[start] = std::experimental::make_optional(0);

    REP (i, N - 1) {
        for (auto edge: g.edges()) {
            auto u = get<0>(edge);
            auto v = get<1>(edge);
            auto d = get<2>(edge);

            if (vertices[u].value() != max && vertices[v].value() > vertices[u].value() + d) {
                vertices[v] = std::experimental::make_optional(vertices[u].value() + d);
            }
        }
    }

    for (auto edge: g.edges()) {
        auto u = get<0>(edge);
        auto v = get<1>(edge);
        auto d = get<2>(edge);
        if (!vertices[u] || (vertices[u].value() != max && vertices[u].value() + d < vertices[v].value())) {
            vertices[v] = std::experimental::optional<i64>();
        }
    }

    return vertices;
}
