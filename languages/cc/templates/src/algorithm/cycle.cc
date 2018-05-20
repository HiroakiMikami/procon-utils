#include <stack>
#ifndef MAIN
#include "common.cc"
#include "graph.cc"
#include "traverse.cc"
#endif

template <typename EdgeLabel, typename Container>
std::experimental::optional<std::vector<Edge<EdgeLabel>>> cycle(const Graph<EdgeLabel, Container> &g, size_t start) {
    using _Edge = Edge<EdgeLabel>;
    std::vector<_Edge> stack;
    std::experimental::optional<std::vector<_Edge>> retval;

    dfs_with_duplicate_vertices(g, {start},
                                [&retval, &stack](const auto &edge) {
                                    while (!stack.empty()) {
                                        auto l = stack.back();
                                        if (get<1>(l) == get<0>(edge)) {
                                            break;
                                        }
                                        stack.pop_back();
                                    }

                                    // find backward path
                                    for (auto &e: stack) {
                                        if (get<0>(e) == get<1>(edge)) {
                                            retval = std::experimental::make_optional(std::vector<_Edge>());
                                        }

                                        if (retval) {
                                            retval.value().push_back(e);
                                        }
                                    }

                                    if (retval) {
                                        retval.value().push_back(edge);
                                        return true;
                                    }

                                    stack.push_back(edge);
                                    return false;
                                },
                                [](const auto &edge) { return true; }
    );

    return retval;
}
