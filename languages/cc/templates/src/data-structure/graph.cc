#include <vector>
#include <unordered_map>
#include <type_traits>
#include <experimental/optional>

#ifndef MAIN
#include "common.cc"
#endif

template <class EdgeLabel>
using Edge = typename std::conditional<std::is_void<EdgeLabel>::value,
        tuple<size_t, size_t>,
        tuple<size_t, size_t, EdgeLabel>
>::type;

namespace internal { namespace graph {
    template <typename Container, typename EdgeLabel>
    struct create {
        Container operator()(size_t vertex_num) const;
    };

    /* AdjacencyList */
    template <typename EdgeLabel>
    using AdjacencyListEntry = typename std::conditional<std::is_void<EdgeLabel>::value, tuple<size_t>, tuple<size_t, EdgeLabel>>::type;

    template <typename EdgeLabel>
    using AdjacencyList = Vector<Vector<AdjacencyListEntry<EdgeLabel>>>;

    template <typename EdgeLabel>
    struct create<AdjacencyList<EdgeLabel>, EdgeLabel> {
        AdjacencyList<EdgeLabel> operator()(size_t vertex_num) const {
            return AdjacencyList<EdgeLabel>(vertex_num);
        }
    };

    template <class EdgeLabel, typename std::enable_if_t<std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static AdjacencyListEntry<EdgeLabel> to_adjacency_list_entry(const Edge<EdgeLabel> &edge) {
        return std::make_tuple(get<1>(edge));
    }
    template <class EdgeLabel, typename std::enable_if_t<!std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static AdjacencyListEntry<EdgeLabel> to_adjacency_list_entry(const Edge<EdgeLabel> &edge) {
        return std::make_tuple(get<1>(edge), get<2>(edge));
    }

    template <typename EdgeLabel>
    static Edge<EdgeLabel> to_edge(size_t from, const AdjacencyListEntry<EdgeLabel> &edge) {
        return std::tuple_cat(std::make_tuple(from), edge);
    }

    template <typename EdgeLabel>
    size_t vertex_size(const AdjacencyList<EdgeLabel> &graph) {
        return graph.size();
    }

    template <typename EdgeLabel>
    auto edges(const AdjacencyList<EdgeLabel> &graph) {
        return iterator_map(
                iterator_flatten(graph),
                [&graph](const auto &it) -> Edge<EdgeLabel> {
                    auto v = std::distance(graph.begin(), it->first);
                    return to_edge<EdgeLabel>(v, *it->second);
                });
    }
    template <typename EdgeLabel>
    auto edges(const AdjacencyList<EdgeLabel> &graph, size_t v1, size_t v2) {
        return iterator_map(iterator_filter(graph[v1],
                                             [&graph, v2](const auto &it) { return get<0>(*it) == v2; }),
                            [&graph, v1](const auto &it) -> Edge<EdgeLabel> { return to_edge<EdgeLabel>(v1, *it); });
    }

    template <typename EdgeLabel>
    auto outgoings(const AdjacencyList<EdgeLabel> &graph, size_t v) {
        return iterator_map(graph[v],
                            [&graph, v](const auto &it) { return to_edge<EdgeLabel>(v, *it); });
    }

    template <typename EdgeLabel>
    bool has_edge(const AdjacencyList<EdgeLabel> &graph, size_t v1, size_t v2) {
        return std::find_if(CTR(graph[v1]), [v2](const auto &elem) { return get<0>(elem) == v2; }) != graph[v1].end();
    }

    template <typename EdgeLabel>
    size_t add_vertex(AdjacencyList<EdgeLabel> &graph) {
        graph.push_back({});
        return graph.size() - 1;
    }
    template <typename EdgeLabel>
    void remove_vertex(AdjacencyList<EdgeLabel> &graph, size_t v) {
        REP (i, graph.size()) {
            if (static_cast<size_t>(i) == v) {
                graph[i].clear();
            } else {
                auto &edges = graph[i];
                edges.erase(
                        remove_if(edges.begin(), edges.end(), [v](const auto &edge) { return get<0>(edge) == v; }),
                        edges.end()
                );
            }
        }
    }

    template <typename EdgeLabel>
    void add_edge(AdjacencyList<EdgeLabel> &graph, const Edge<EdgeLabel>& edge) {
        graph[get<0>(edge)].push_back(to_adjacency_list_entry<EdgeLabel>(edge));
    }
    template <typename EdgeLabel>
    void remove_edge(AdjacencyList<EdgeLabel> &graph, const Edge<EdgeLabel>& edge) {
        graph[get<0>(edge)].erase(
                remove(graph[get<0>(edge)].begin(), graph[get<0>(edge)].end(), to_adjacency_list_entry<EdgeLabel>(edge)),
                graph[get<0>(edge)].end()
        );
    }
    template <typename EdgeLabel>
    void remove_edge(AdjacencyList<EdgeLabel> &graph, size_t n1, size_t n2) {
        graph[n1].erase(
                remove_if(
                        graph[n1].begin(), graph[n1].end(),
                        [n2](const auto &edge) { return get<0>(edge) == n2; }
                ),
                graph[n1].end()
        );
    }

    /* AdjacencyMatrix */
    template <typename EdgeLabel>
    using AdjacencyMatrixElement = typename std::conditional<
            std::is_void<EdgeLabel>::value, bool,
            std::experimental::optional<EdgeLabel>>::type;

    template <typename EdgeLabel>
    using AdjacencyMatrix= Matrix<AdjacencyMatrixElement<EdgeLabel>, 2>;
    template <typename EdgeLabel>
    struct create<AdjacencyMatrix<EdgeLabel>, EdgeLabel> {
        AdjacencyMatrix<EdgeLabel> operator()(size_t vertex_num) const {
            return make_matrix<AdjacencyMatrixElement<EdgeLabel>, 2>({vertex_num, vertex_num});
        }
    };

    template<class EdgeLabel, typename std::enable_if_t<std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static Edge<EdgeLabel> to_edge(size_t n1, size_t n2, bool element __attribute__((unused))) {
        return std::make_tuple(n1, n2);
    }
    template<class EdgeLabel, typename std::enable_if_t<!std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static Edge<EdgeLabel> to_edge(
            size_t n1, size_t n2,
            const std::experimental::optional<typename std::enable_if_t<!std::is_void<EdgeLabel>::value, EdgeLabel>> &element
    ) {
        return std::make_tuple(n1, n2, element.value());
    }

    template<class EdgeLabel, typename std::enable_if_t<std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static bool to_adjacency_matrix_element(const Edge<EdgeLabel> &edge __attribute__((unused))) {
        return true;
    }
    template<class EdgeLabel, typename std::enable_if_t<!std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static std::experimental::optional<EdgeLabel> to_adjacency_matrix_element(const Edge<EdgeLabel> &edge) {
        return std::experimental::make_optional(get<2>(edge));
    }

    template <typename EdgeLabel>
    size_t vertex_size(const AdjacencyMatrix<EdgeLabel> &matrix) {
        return matrix.size();
    }

    template <typename EdgeLabel>
    auto has_edge(const AdjacencyMatrix<EdgeLabel> &matrix, size_t n1, size_t n2) {
        return static_cast<bool>(matrix[n1][n2]);
    }

    template <typename EdgeLabel>
    auto edges(const AdjacencyMatrix<EdgeLabel> &matrix) {
        return iterator_map(iterator_filter(iterator_flatten(matrix),
                                            [](const auto &elem) { return static_cast<bool>(*elem->second); }),
                            [&matrix](const auto &elem) {
                                auto v = distance(matrix.begin(), elem->first);
                                auto u = distance(elem->first->begin(), elem->second);
                                return to_edge<EdgeLabel>(v, u, *elem->second);
                            });
    }
    template <typename EdgeLabel>
    auto edges(const AdjacencyMatrix<EdgeLabel> &matrix, size_t n1, size_t n2) {
        if (has_edge<EdgeLabel>(matrix, n1, n2)) {
            return Vector<Edge<EdgeLabel>>(1, to_edge<EdgeLabel>(n1, n2, matrix[n1][n2]));
        } else {
            return Vector<Edge<EdgeLabel>>();
        }
    }

    template <typename EdgeLabel>
    auto outgoings(const AdjacencyMatrix<EdgeLabel> &matrix, size_t n) {
        return iterator_map(iterator_filter(iterator_map(matrix[n],
                                                         [&matrix, n](const auto &elem) -> std::experimental::optional<Edge<EdgeLabel>> {
                                                             if (*elem) {
                                                                 auto u = distance(matrix[n].begin(), elem);
                                                                 return std::experimental::make_optional(to_edge<EdgeLabel>(n, u, *elem));
                                                             } else {
                                                                 return std::experimental::optional<Edge<EdgeLabel>>();
                                                             }
                                                         }),
                                            [](const auto &elem) { return static_cast<bool>(*elem); }),
                            [](const auto &elem) { return (*elem).value(); });
    }

    template <typename EdgeLabel>
    void add_edge(AdjacencyMatrix<EdgeLabel> &matrix, const Edge<EdgeLabel> &edge) {
        assert(!matrix[get<0>(edge)][get<1>(edge)]);
        matrix[get<0>(edge)][get<1>(edge)] = to_adjacency_matrix_element<EdgeLabel>(edge);
    }

    template <typename EdgeLabel>
    void remove_edge(AdjacencyMatrix<EdgeLabel> &matrix, const Edge<EdgeLabel> &edge) {
        auto elem = to_adjacency_matrix_element<EdgeLabel>(edge);
        if (matrix[get<0>(edge)][get<1>(edge)] == elem) {
            matrix[get<0>(edge)][get<1>(edge)] = AdjacencyMatrixElement<EdgeLabel>();
        }
    }

    template <typename EdgeLabel>
    void remove_edge(AdjacencyMatrix<EdgeLabel> &matrix, size_t n1, size_t n2) {
        matrix[n1][n2] = AdjacencyMatrixElement<EdgeLabel>();
    }

    template <typename EdgeLabel>
    size_t add_vertex(AdjacencyMatrix<EdgeLabel> &matrix) {
        auto n = matrix.size();
        REP (i, matrix.size()) {
            matrix[i].resize(n + 1);
        }
        matrix.emplace(n + 1);
        return n;
    }

    template <typename EdgeLabel>
    void remove_vertex(AdjacencyMatrix<EdgeLabel> &matrix, size_t n) {
        REP (i, matrix.size()) {
            REP (j, matrix.size()) {
                if (static_cast<size_t>(i) == n || static_cast<size_t>(j) == n) {
                    matrix[i][j] = AdjacencyMatrixElement<EdgeLabel>();
                }
            }
        }
    }
}}

template <typename _EdgeLabel, typename Container>
struct Graph {
    using EdgeLabel = _EdgeLabel;

    Graph() {}
    Graph(const Container& c) : container(c) {}
    Graph(size_t vertex_num) : container(internal::graph::create<Container, EdgeLabel>()(vertex_num)) {}

    size_t vertices_size() const {
        return internal::graph::vertex_size<EdgeLabel>(this->container);
    }

    auto edges() const {
        return internal::graph::edges<EdgeLabel>(this->container);
    }
    auto edges(size_t n1, size_t n2) const {
        return internal::graph::edges<EdgeLabel>(this->container, n1, n2);
    }

    auto outgoings(size_t n) const {
        return internal::graph::outgoings<EdgeLabel>(this->container, n);
    }

    auto has_edge(size_t n1, size_t n2) const {
        return internal::graph::has_edge<EdgeLabel>(this->container, n1, n2);
    }

    void add_edge(const Edge<EdgeLabel>& edge) {
        internal::graph::add_edge<EdgeLabel>(this->container, edge);
    }
    void remove_edge(const Edge<EdgeLabel>& edge) {
        internal::graph::remove_edge<EdgeLabel>(this->container, edge);
    }
    void remove_edge(size_t n1, size_t n2) {
        internal::graph::remove_edge<EdgeLabel>(this->container, n1, n2);
    }
    size_t add_vertex() {
        return internal::graph::add_vertex<EdgeLabel>(this->container);
    }
    void remove_vertex(size_t n) {
        internal::graph::remove_vertex<EdgeLabel>(this->container, n);
    }

    void to_undirected() {
        std::vector<Edge<EdgeLabel>> es;
        for (auto edge: this->edges()) {
            std::swap(get<0>(edge), get<1>(edge));
            es.push_back(edge);
        }

        EACH (edge, es) {
            this->add_edge(edge);
        }
    }
    Container container;
};

template <typename EdgeLabel>
using AdjacencyList = Graph<EdgeLabel, internal::graph::AdjacencyList<EdgeLabel>>;
using SimpleAdjacencyList = AdjacencyList<void>;
using WeightedAdjacencyList = AdjacencyList<i64>;
using UnsignedWeightedAdjacencyList = AdjacencyList<u64>;

template <typename EdgeLabel>
using AdjacencyMatrix = Graph<EdgeLabel, internal::graph::AdjacencyMatrix<EdgeLabel>>;
using SimpleAdjacencyMatrix = AdjacencyMatrix<void>;
using WeightedAdjacencyMatrix = AdjacencyMatrix<i64>;
using UnsignedWeightedAdjacencyMatrix = AdjacencyMatrix<u64>;

namespace internal {
    template <typename Label, typename Container>
    struct oneline<Graph<Label, Container>> {
        std::string operator()(const Graph<Label, Container> &g) const {
            Vector<size_t> V;
            REP (i, g.vertices_size()) {
                V.push_back(i);
            }
            Vector<Edge<Label>> E;
            EACH_V(e, g.edges()) {
                E.push_back(e);
            }
            using P = pair<decltype(V), decltype(E)>;
            return oneline<P>()(make_pair(V, E));
        }
    };
}
