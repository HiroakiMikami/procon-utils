#include <stack>
#include <queue>
#include <deque>

#ifndef MAIN
#include "common.cc"
#include "graph.cc"
#endif

namespace graph {
    template<typename EdgeLabel, typename GraphContainer,
            typename Container, typename Push, typename Pop,
            typename Visit, typename FilterNot>
    void traverse(const Graph<EdgeLabel, GraphContainer> &g,
                  const std::vector<size_t> &start,
                  const Push &push = Push(), const Pop &pop = Pop(),
                  const Visit &visit = Visit(), const FilterNot &filter_not = FilterNot()) {
                  Container container;

        using Optional = std::optional<Edge<EdgeLabel>>;
        using std::make_optional;
        for (const auto &s : start) {
            push(container, make_pair(Optional{}, s));
        }

        while (!container.empty()) {
            std::pair<Optional, size_t> x = pop(container);
            if (filter_not(x.first, x.second)) {
                continue;
            }

            if (visit(x.first, x.second)) {
                break;
            }

            for (auto edge: g.outgoings(x.second)) {
                push(container, make_pair(make_optional(edge), get<1>(edge)));
            }
        }
    }

    template<typename EdgeLabel, typename GraphContainer, typename Visit, typename FilterNot>
    void dfs(const Graph<EdgeLabel, GraphContainer> &g, const std::vector<size_t> &start,
             const Visit &visit = Visit(), const FilterNot &filter_not = FilterNot()) {
        using Optional = std::optional<Edge<EdgeLabel>>;
        auto push = [](std::stack<std::pair<Optional, size_t>> &container, const std::pair<Optional, size_t> &elem) { container.push(elem); };
        auto pop = [](std::stack<std::pair<Optional, size_t>> &container) {
            auto retval = container.top();
            container.pop();
            return retval;
        };

        traverse<EdgeLabel, GraphContainer,
                std::stack<std::pair<Optional, size_t>>, decltype(push), decltype(pop),
                Visit, FilterNot>(g, start, push, pop, visit, filter_not);
    }

    template<typename EdgeLabel, typename GraphContainer, typename Visit, typename FilterNot>
    void bfs(const Graph<EdgeLabel, GraphContainer> &g, const std::vector<size_t> &start,
             const Visit &visit = Visit(), const FilterNot &filter_not = FilterNot()) {
        using Optional = std::optional<Edge<EdgeLabel>>;
        auto push = [](std::queue<std::pair<Optional, size_t>> &container, const std::pair<Optional, size_t> &elem) { container.push(elem); };
        auto pop = [](std::queue<std::pair<Optional, size_t>> &container) {
            auto retval = container.front();
            container.pop();
            return retval;
        };

        traverse<EdgeLabel, GraphContainer,
                std::queue<std::pair<Optional, size_t>>, decltype(push), decltype(pop),
                Visit, FilterNot>(g, start, push, pop, visit, filter_not);
    }

    template<typename EdgeLabel, typename GraphContainer, typename Visit, typename FilterNot>
    void zero_one_bfs(const Graph<EdgeLabel, GraphContainer> &g, const std::vector<size_t> &start,
             const Visit &visit = Visit(), const FilterNot &filter_not = FilterNot()) {
        using Optional = std::optional<Edge<EdgeLabel>>;
        auto push = [](std::deque<std::pair<Optional, size_t>> &container, const std::pair<Optional, size_t> &elem) {
            if (elem.first) {
                if (get<2>(elem.first.value())) {
                    container.push_back(elem);
                } else {
                    container.push_front(elem);
                }
            } else {
                container.push_back(elem);
            }
        };
        auto pop = [](std::deque<std::pair<Optional, size_t>> &container) {
            auto retval = container.front();
            container.pop_front();
            return retval;
        };

        traverse<EdgeLabel, GraphContainer,
                std::deque<std::pair<Optional, size_t>>, decltype(push), decltype(pop),
                Visit, FilterNot>(g, start, push, pop, visit, filter_not);
    }

    namespace internal {
        template<typename EdgeLabel, typename Container>
        struct FilterDuplicated {
            mutable std::vector<bool> visited;
            FilterDuplicated(const Graph<EdgeLabel, Container> &g) : visited(g.vertices_size(), false) {}

            bool operator()(const std::optional<Edge<EdgeLabel>> &edge __attribute__((unused)), size_t v) const {
                if (visited[v]) {
                    return true;
                }

                visited[v] = true;
                return false;
            }
        };
    }
    template <typename EdgeLabel, typename Container>
    auto filter_nothing(const Graph<EdgeLabel, Container>& g __attribute__((unused))) {
        auto retval = [](const auto &edge __attribute__((unused)), size_t v __attribute__((unused))) { return false; };
        return retval;
    }

    template<typename EdgeLabel, typename Container>
    internal::FilterDuplicated<EdgeLabel, Container> filter_duplicated(const Graph<EdgeLabel, Container> &g) {
        return internal::FilterDuplicated<EdgeLabel, Container>(g);
    };
}
