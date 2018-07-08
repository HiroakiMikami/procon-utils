#include <limits>
#include <experimental/optional>

#ifndef MAIN
#include "common.cc"
#include "data-structure/graph.cc"
#include "data-structure/union_find.cc"
#endif

namespace graph {
    template <typename EdgeLabel, typename Container>
    std::pair<EdgeLabel, std::vector<Edge<EdgeLabel>>> prim(const Graph<EdgeLabel, Container> &g, size_t start) {
        using std::vector;
        using std::unordered_set;
        using optional = std::experimental::optional<Edge<EdgeLabel>>;
        using std::experimental::make_optional;

        std::unordered_set<size_t> V{start};
        std::vector<Edge<EdgeLabel>> E;
        EdgeLabel cost = 0;

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

    template <typename EdgeLabel, typename Container>
    std::pair<EdgeLabel, std::vector<Edge<EdgeLabel>>> kruskal(const Graph<EdgeLabel, Container> &g) {
        using std::vector;
        using std::unordered_set;
        using std::experimental::make_optional;
        using _Edge = Edge<EdgeLabel>;

        UnionFind clusters(g.vertices_size());

        std::vector<_Edge> E;
        EdgeLabel cost = 0;

        auto cmp = [&](const auto &e1, const auto &e2) { return get<2>(e1) > get<2>(e2); };
        PriorityQueue<_Edge, decltype(cmp)> Q(cmp);

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
}
