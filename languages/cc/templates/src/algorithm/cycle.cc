#include <stack>
#ifndef MAIN
#include "common.cc"
#include "graph.cc"
#include "traverse.cc"
#endif

namespace graph {
    template <typename EdgeLabel, typename Container>
    std::optional<std::vector<Edge<EdgeLabel>>> cycle(const Graph<EdgeLabel, Container> &g, size_t start) {
        using _Edge = Edge<EdgeLabel>;
        std::vector<_Edge> stack;
        std::optional<std::vector<_Edge>> retval;

        dfs(g, {start},
                 [&retval, &stack](const auto &edge_opt, size_t v __attribute__((unused))) {
                     if (!edge_opt) return false;
                     auto edge = edge_opt.value();
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
                             retval = std::make_optional(std::vector<_Edge>());
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
                 filter_nothing(g));

        return retval;
    }
}
