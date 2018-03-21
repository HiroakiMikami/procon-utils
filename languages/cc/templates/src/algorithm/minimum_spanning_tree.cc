#include <limits>
#include <experimental/optional>

#ifndef MAIN
#include "common.cc"
#include "data-structure/graph.cc"
#include "data-structure/union_find.cc"
#endif

template <class Graph>
std::pair<typename Graph::EdgeLabel, std::vector<Edge<typename Graph::EdgeLabel>>> prim(const Graph &g, size_t start) {
    using std::vector;
    using std::unordered_set;
    using optional = std::experimental::optional<Edge<typename Graph::EdgeLabel>>;
    using std::experimental::make_optional;

    std::unordered_set<size_t> V{start};
    std::vector<Edge<typename Graph::EdgeLabel>> E;
    typename Graph::EdgeLabel cost = 0;

    while (V.size() != g.vertices_size()) {
        // TODO 隣接リストのときはもっと効率よく実装できる
        auto e = optional();

        for (auto v1: V) {
            for (auto edge: g.outgoings(v1)) {
                if (V.find(get<1>(edge)) != V.end()) {
                    continue ;
                }

                if (!e || get<2>(e.value()) > get<2>(edge)) {
                    e = make_optional(edge);
                }
            }
        }

        if (e) {
            V.insert(get<1>(e.value()));
            E.push_back(e.value());
            cost += get<2>(e.value());
        }
    }

    return std::make_pair(cost, E);
}

template <class Graph>
std::pair<typename Graph::EdgeLabel, std::vector<Edge<typename Graph::EdgeLabel>>> kruskal(const Graph &g) {
    using std::vector;
    using std::unordered_set;
    using optional = std::experimental::optional<Edge<typename Graph::EdgeLabel>>;
    using std::experimental::make_optional;
    using _Edge = Edge<typename Graph::EdgeLabel>;

    UnionFind clusters(g.vertices_size());

    std::vector<_Edge> E;
    typename Graph::EdgeLabel cost = 0;

    auto cmp = [&](const auto &e1, const auto &e2) { return get<2>(e1) > get<2>(e2); };
    priority_queue<_Edge, vector<_Edge>, decltype(cmp)> Q(cmp);

    for (auto edge: g.edges()) {
        // undirected graphを前提
        if (get<0>(edge) <= get<1>(edge)) {
            Q.push(edge);
        }
    }

    while (E.size() < g.vertices_size() - 1) {
        auto edge = Q.top();
        Q.pop();

        if (!clusters.is_same(get<0>(edge), get<1>(edge))) {
            E.push_back(edge);
            cost += get<2>(edge);
            clusters.merge(get<0>(edge), get<1>(edge));
        }
    }

    return std::make_pair(cost, E);
}