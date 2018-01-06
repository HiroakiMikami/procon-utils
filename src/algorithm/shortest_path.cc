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

    bool operator==(const CostWithPreviousVertex<Cost> &rhs) const {
        return this->cost == rhs.cost && this->previous_vertex == rhs.previous_vertex;
    }
    bool operator!=(const CostWithPreviousVertex<Cost> &rhs) const {
        return !(*this == rhs);
    }
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
std::vector<std::vector<CostWithPreviousVertex<typename Graph::EdgeLabel>>> warshall_floyd(const Graph &g) {
    using std::vector;
    using std::experimental::optional;
    using std::experimental::make_optional;
    using C = CostWithPreviousVertex<typename Graph::EdgeLabel>;

    auto max = std::numeric_limits<typename Graph::EdgeLabel>::max();
    CostWithPreviousVertex<typename Graph::EdgeLabel>(0, make_optional(0));

    auto N = g.vertices_size();
    vector<vector<C>> distance(N, vector<C>(N, C(max, optional<size_t>())));

    REP (k, N) {
        distance[k][k] = C(0, optional<size_t>());
        for (auto edge: g.outgoings(k)) {
            distance[k][get<1>(edge)] = C(get<2>(edge), make_optional(k));
        }
    }

    REP (k, N) {
        REP (i, N) {
            REP (j, N) {
                if (distance[i][k].cost != max && distance[k][j].cost != max &&
                    distance[i][j].cost > distance[i][k].cost + distance[k][j].cost) {
                    auto prev_opt = distance[k][j].previous_vertex;
                    auto prev = prev_opt.value_or(k);
                    distance[i][j] = C(distance[i][k].cost + distance[k][j].cost, make_optional(prev));
                }
            }
        }
    }

    return distance;
}
