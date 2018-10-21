#include <limits>
#include <experimental/optional>

#ifndef MAIN
#include "common.cc"
#include "graph.cc"
#endif

namespace graph {
    template <typename EdgeLabel, typename Container, typename Flow = EdgeLabel>
    Flow ford_fulkerson(const Graph<EdgeLabel, Container> &g, size_t start, size_t goal) { // TODO use traverse
        auto g_ = g;
        Vector<bool> used(g.vertices_size(), false);
        auto dfs_rec = [&](size_t s, size_t e, Flow f, auto dfs) -> Flow {
            if (s == e) return f;
            used[s] = true;
            for (auto edge: g_.outgoings(s)) {
                auto to = get<1>(edge);
                auto cap = get<2>(edge);
                if (!used[to] and cap > 0) {
                    auto d = dfs(to, e, std::min<Flow>(f, cap), dfs);
                    if (d > 0) {
                        // TODO performance
                        g_.remove_edge(s, to);
                        g_.add_edge(make_tuple(s, to, static_cast<EdgeLabel>(cap - d)));
                        auto reverse = g.edges(to, s);
                        auto rev_cap = (reverse.begin() == reverse.end()) ? 0 : get<2>(*reverse.begin());
                        g_.remove_edge(to, s);
                        g_.add_edge(make_tuple(to, s, static_cast<EdgeLabel>(rev_cap + d)));
                        return d;
                    }
                }
            }
            return static_cast<Flow>(0);
        };
        Flow flow = 0;
        while (true) {
            used = Vector<bool>(g.vertices_size(), false);
            auto f = dfs_rec(start, goal, std::numeric_limits<Flow>::max(), dfs_rec);
            if (f == 0) return flow;
            flow += f;
        }
    }
}
