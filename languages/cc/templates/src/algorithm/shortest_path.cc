#include <limits>
#include <experimental/optional>

#ifndef MAIN
#include "common.cc"
#include "graph.cc"
#endif

namespace graph {
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

    template <typename EdgeLabel, typename Container>
    std::vector<CostWithPreviousVertex<EdgeLabel>> dijkstra(const Graph<EdgeLabel, Container> &g, size_t start) {
        using C = CostWithPreviousVertex<EdgeLabel>;
        using optional = std::experimental::optional<size_t>;
        using std::experimental::make_optional;
        using std::vector;
        using std::priority_queue;
        using pair = std::pair<size_t, EdgeLabel>;
        using std::make_pair;

        auto max = std::numeric_limits<EdgeLabel>::max();
        auto N = g.vertices_size();
        auto distance = vector<C>(N, C(max, optional()));
        auto is_used = vector<bool>(N, false);
        auto cmp = [&](const pair &v1, const pair &v2) { return v1.second > v2.second; };
        auto Q = priority_queue<pair, vector<pair>, decltype(cmp)>(cmp);

        distance[start] = C(0, optional());
        REP (i, N) {
            Q.push(make_pair(i, distance[i].cost));
        }

        while (!Q.empty()) {
            auto u = Q.top().first;
            Q.pop();

            if (is_used[u]) {
                continue ;
            }
            is_used[u] = true;

            for (auto edge: g.outgoings(u)) {
                auto v = get<1>(edge);
                auto weight = get<2>(edge);
                if (distance[u].cost != max && distance[v].cost > distance[u].cost + weight) {
                    distance[v] = C(distance[u].cost + weight, u);
                    Q.push(make_pair(v, distance[v].cost));
                }
            }
        }

        return distance;
    }

    template <typename EdgeLabel, typename Container>
    std::vector<std::experimental::optional<CostWithPreviousVertex<EdgeLabel>>> bellman_ford(const Graph<EdgeLabel, Container> &g, size_t start) {
        using std::experimental::optional;
        using std::experimental::make_optional;

        auto max = std::numeric_limits<EdgeLabel>::max();
        auto N = g.vertices_size();

        using C = CostWithPreviousVertex<EdgeLabel>;
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

    template <typename EdgeLabel, typename Container>
    std::vector<std::vector<CostWithPreviousVertex<EdgeLabel>>> warshall_floyd(const Graph<EdgeLabel, Container> &g) {
        using std::vector;
        using std::experimental::optional;
        using std::experimental::make_optional;
        using C = CostWithPreviousVertex<EdgeLabel>;

        auto max = std::numeric_limits<EdgeLabel>::max();
        CostWithPreviousVertex<EdgeLabel>(0, make_optional(0));

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
}
