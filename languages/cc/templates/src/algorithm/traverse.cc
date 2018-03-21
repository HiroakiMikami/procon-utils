#include <stack>
#include <queue>
#include <deque>

#ifndef MAIN
#include "common.cc"
#include "graph.cc"
#endif

template <class Graph, class F1, class F2, class Container, class Push, class Pop>
void traverse(const Graph &g, const std::vector<size_t> &start,
              Container &container, const Push &push, const Pop &pop,
              const F1 &process,
              const F2 &is_added = [](const Edge<typename Graph::EdgeLabel> &e) { return true; }) {
    for (const auto &n: start) {
        for (auto edge: g.outgoings(n)) {
            if (is_added(edge)) push(container, edge);
        }
    }

    while (!container.empty()) {
        auto x = pop(container);

        if (process(x)) {
            return ;
        }

        for (const auto edge: g.outgoings(get<1>(x))) {
            if (is_added(edge)) push(container, edge);
        }
    }
}

template <class Graph, class F1, class F2>
void dfs_with_duplicate_vertices(const Graph &g, const std::vector<size_t> &start, const F1 &process,
                                 const F2 &is_added = [](const Edge<typename Graph::EdgeLabel> &e) { return true; }) {
    using _Edge = Edge<typename Graph::EdgeLabel>;
    std::stack<_Edge> s;

    traverse(g, start, s,
             [](auto &s, auto &edge) { s.push(edge); },
             [](auto &s) {
                 auto x = s.top();
                 s.pop();
                 return x;
             },
             process, is_added);
}
template <class Graph, class F1, class F2>
void dfs(const Graph &g, const std::vector<size_t> &start, const F1 &process,
         const F2 &is_added = [](const Edge<typename Graph::EdgeLabel> &e) { return true; }) {
    std::vector<bool> is_visited(g.vertices_size(), false);
    dfs_with_duplicate_vertices(g, start, process,
                                [&is_added, &is_visited](const auto &edge) {
                                    if (!is_added(edge)) {
                                        return false;
                                    }
                                    auto retval = !is_visited[get<1>(edge)];
                                    is_visited[get<1>(edge)] = true;
                                    return retval;
                                });
}
template <class Graph, class F1>
void dfs(const Graph &g, const std::vector<size_t> &start, const F1 &process) {
    dfs(g, start, process, [](const auto &edge) { return true; });
}

template <class Graph, class F1, class F2>
void bfs_with_duplicate_vertices(const Graph &g, const std::vector<size_t> &start, const F1 &process,
                                 const F2 &is_added = [](const Edge<typename Graph::EdgeLabel> &e) { return true; }) {
    using _Edge = Edge<typename Graph::EdgeLabel>;
    std::queue<_Edge> s;

    traverse(g, start, s,
             [](auto &s, auto &edge) { s.push(edge); },
             [](auto &s) {
                 auto x = s.front();
                 s.pop();
                 return x;
             },
             process, is_added);
}
template <class Graph, class F1, class F2>
void bfs(const Graph &g, const std::vector<size_t> &start, const F1 &process,
         const F2 &is_added = [](const Edge<typename Graph::EdgeLabel> &e) { return true; }) {
    std::vector<bool> is_visited(g.vertices_size(), false);
    bfs_with_duplicate_vertices(g, start, process,
                                [&is_added, &is_visited](const auto &edge) {
                                    if (!is_added(edge)) {
                                        return false;
                                    }
                                    auto retval = !is_visited[get<1>(edge)];
                                    is_visited[get<1>(edge)] = true;
                                    return retval;
                                });
}
template <class Graph, class F1>
void bfs(const Graph &g, const std::vector<size_t> &start, const F1 &process) {
    bfs(g, start, process, [](const auto &edge) { return true; });
}

template <class Graph, class F1, class F2>
void zero_one_bfs_with_duplicate_vertices(const Graph &g, const std::vector<size_t> &start, const F1 &process,
                                 const F2 &is_added = [](const Edge<typename Graph::EdgeLabel> &e) { return true; }) {
    using _Edge = Edge<typename Graph::EdgeLabel>;
    std::deque<_Edge> s;

    traverse(g, start, s,
             [](auto &s, auto &edge) {
                 if (get<2>(edge)) {
                     s.push_back(edge);
                 } else {
                     s.push_front(edge);
                 }
             },
             [](auto &s) {
                 auto x = s.front();
                 s.pop_front();
                 return x;
             },
             process, is_added);
}
template <class Graph, class F1, class F2>
void zero_one_bfs(const Graph &g, const std::vector<size_t> &start, const F1 &process,
         const F2 &is_added = [](const Edge<typename Graph::EdgeLabel> &e) { return true; }) {
    std::vector<bool> is_visited(g.vertices_size(), false);
    zero_one_bfs_with_duplicate_vertices(g, start, process,
                                [&is_added, &is_visited](const auto &edge) {
                                    if (!is_added(edge)) {
                                        return false;
                                    }
                                    auto retval = !is_visited[get<1>(edge)];
                                    is_visited[get<1>(edge)] = true;
                                    return retval;
                                });
}
template <class Graph, class F1>
void zero_one_bfs(const Graph &g, const std::vector<size_t> &start, const F1 &process) {
    zero_one_bfs(g, start, process, [](const auto &edge) { return true; });
}
