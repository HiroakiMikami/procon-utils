#include <limits>
#include <experimental/optional>

#ifndef MAIN
#include "common.cc"
#include "graph.cc"
#endif

template <class Cost>
struct CostWithPreviousVertex {
    CostWithPreviousVertex(Cost cost, std::experimental::optional<size_t> previous_vertex)
            : cost(cost), previous_vertex(previous_vertex) {}
    Cost cost;
    std::experimental::optional<size_t> previous_vertex;
};

template <class Graph>
std::vector<std::experimental::optional<CostWithPreviousVertex<typename Graph::EdgeLabel>>> bellman_ford(const Graph &g, size_t start) {
    using std::experimental::optional;
    using std::experimental::make_optional;

    auto max = std::numeric_limits<typename Graph::EdgeLabel>::max();
    auto N = g.vertices_size();

    using C = CostWithPreviousVertex<typename Graph::EdgeLabel>;
    auto vertices = std::vector<optional<C>>(N, make_optional(C(max, optional<size_t>())));
    vertices[start] = make_optional(C(0, optional<size_t>()));

    REP (i, N - 1) {
        for (auto edge: g.edges()) {
            auto u = get<0>(edge);
            auto v = get<1>(edge);
            auto d = get<2>(edge);

            if (vertices[u].value().cost != max &&
                vertices[v].value().cost > vertices[u].value().cost + d) {
                vertices[v] = make_optional(C(vertices[u].value().cost + d, make_optional(u)));
            }
        }
    }

    for (auto edge: g.edges()) {
        auto u = get<0>(edge);
        auto v = get<1>(edge);
        auto d = get<2>(edge);
        if (!vertices[u] ||
            (vertices[u].value().cost != max && vertices[u].value().cost + d < vertices[v].value().cost)) {
            vertices[v] = optional<C>();
        }
    }

    return vertices;
}

template <class Graph>
std::vector<std::vector<i64>> warshall_floyd(const Graph &g) {
    auto max = std::numeric_limits<i64>::max();
    auto N = g.vertices_size();
    std::vector<std::vector<i64>> distance(N, std::vector<i64>(N, max));

    REP (k, N) {
        distance[k][k] = 0;
        for (auto edge: g.outgoings(k)) {
            distance[k][get<1>(edge)] = get<2>(edge);
        }
    }

    REP (k, N) {
        REP (i, N) {
            REP (j, N) {
                if (distance[i][k] != max) {
                    distance[i][j] = std::min(distance[i][j], distance[i][k] + distance[k][j]);
                }
            }
        }
    }

    return distance;
}
