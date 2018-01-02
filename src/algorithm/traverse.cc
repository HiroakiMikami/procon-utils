#include <stack>

#ifndef MAIN
#include "common.cc"
#include "graph.cc"
#endif

template <class Graph, class F1, class F2>
void dfs_with_duplicate_vertices(const Graph &g, const std::vector<size_t> &start,
                                 const F1 &process, const F2 &is_added) {
    using _Edge = Edge<typename Graph::EdgeLabel>;
    std::stack<_Edge> s;
    for (const auto &n: start) {
        for (auto edge: g.outgoings(n)) {
            if (is_added(edge)) s.push(edge);
        }
    }

    while (!s.empty()) {
        auto x = s.top();
        s.pop();

        if (process(x)) {
            return ;
        }

        for (const auto edge: g.outgoings(get<1>(x))) {
            if (is_added(edge)) s.push(edge);
        }
    }
}
template <class Graph, class F1, class F2>
void dfs(const Graph &g, const std::vector<size_t> &start,
         const F1 &process, const F2 &is_added) {
    std::vector<bool> is_visited(g.vertices_size(), false);
    dfs_with_duplicate_vertices(g, start, process,
                                [&is_added, &is_visited](const auto &edge) {
                                    auto retval = !is_visited[get<1>(edge)];
                                    is_visited[get<1>(edge)] = true;
                                    return retval;
                                });
}
