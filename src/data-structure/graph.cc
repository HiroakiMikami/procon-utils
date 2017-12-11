#include <vector>
#include <unordered_map>
#include <type_traits>
#include <experimental/optional>

#ifndef MAIN
#include "common.cc"
#endif

template <class EdgeLabel>
using Edge = typename std::conditional<std::is_void<EdgeLabel>::value, tuple<size_t, size_t>, tuple<size_t, size_t, EdgeLabel>>::type;

template <
        class _EdgeLabel,
        class List = vector<typename conditional<is_void<_EdgeLabel>::value, tuple<size_t>, tuple<size_t, _EdgeLabel>>::type>,
        class Container = vector<List>>
struct AdjacencyList {
    using EdgeLabel = _EdgeLabel;

    AdjacencyList() {}
    AdjacencyList(size_t vertex_num) : m_list(vertex_num) {}

    const size_t vertices_size() const {
        return this->m_list.size();
    }

    template <class Func>
    void edges(const Func& func) const {
        REP(i, this->m_list.size()) {
            FORE(edge, this->m_list[i]) {
                if (func(to_edge(i, edge))) {
                    return ;
                }
            }
        }
    }
    template <class Func>
    void edges(size_t n1, size_t n2, const Func &func) const {
        FORE(edge, this->m_list[n1]) {
            if (get<0>(edge) == n2) {
                if (func(to_edge(n1, edge))) {
                    return ;
                }
            }
        }
    }

    template <class Func>
    void outgoings(size_t n, const Func &func) const {
        FORE(edge, this->m_list[n]) {
            if (func(to_edge(n, edge))) {
                return ;
            }
        }
    }

    auto has_edge(size_t n1, size_t n2) const {
        return find_if(
                this->m_list[n1].begin(), this->m_list[n1].end(),
                [n2](const auto &edge) { return get<0>(edge) == n2; }
        ) != this->m_list[n1].end();
    }

    void add_edge(const Edge<EdgeLabel>& edge) {
        this->m_list[get<0>(edge)].push_back(to_entry<EdgeLabel>(edge));
    }
    void remove_edge(const Edge<EdgeLabel>& edge) {
        this->m_list[get<0>(edge)].erase(
                remove(this->m_list[get<0>(edge)].begin(), this->m_list[get<0>(edge)].end(), to_entry<EdgeLabel>(edge)),
                this->m_list[get<0>(edge)].end()
        );
    }
    void remove_edge(size_t n1, size_t n2) {
        this->m_list[n1].erase(
                remove_if(
                        this->m_list[n1].begin(), this->m_list[n1].end(),
                        [n2](const auto &edge) { return get<0>(edge) == n2; }
                ),
                this->m_list[n1].end()
        );
    }
    size_t add_vertex() {
        this->m_list.push_back({});
        return this->m_list.size() - 1;
    }
    void remove_vertex(size_t n) {
        for (auto i = 0; i < this->m_list.size(); ++i) {
            if (i == n) {
                this->m_list[i].clear();
            } else {
                auto &edges = this->m_list[i];
                edges.erase(
                        remove_if(edges.begin(), edges.end(), [n](const auto &edge) { return get<0>(edge) == n; }),
                        edges.end()
                );
            }
        }
    }

    void to_undirected() {
        std::vector<Edge<EdgeLabel>> es;
        this->edges([&es](const auto &edge) {
            auto e = edge;
            std::swap(get<0>(e), get<1>(e));
            es.push_back(e);

            return false;
        });

        FORE (edge, es) {
            this->add_edge(edge);
        }
    }
private:
    Container m_list;

    using EdgeEntry = typename conditional<is_void<EdgeLabel>::value, tuple<size_t>, tuple<size_t, EdgeLabel>>::type;

    template <class EdgeLabel, typename std::enable_if_t<std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static EdgeEntry to_entry(const Edge<EdgeLabel> &edge) {
        return std::make_tuple(get<1>(edge));
    }
    template <class EdgeLabel, typename std::enable_if_t<!std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static EdgeEntry to_entry(const Edge<EdgeLabel> &edge) {
        return std::make_tuple(get<1>(edge), get<2>(edge));
    }

    static Edge<EdgeLabel> to_edge(size_t from, const EdgeEntry &edge) {
        return std::tuple_cat(std::make_tuple(from), edge);
    }
};

using SimpleAdjacencyList = AdjacencyList<void>;
using WeightedAdjacencyList = AdjacencyList<i64>;
using UnsignedWeightedAdjacencyList = AdjacencyList<u64>;

template <
        class _EdgeLabel,
        class Element = typename std::conditional<
                std::is_void<_EdgeLabel>::value, bool,
                std::experimental::optional<_EdgeLabel>>::type,
        class Row = std::vector<Element>,
        class Matrix = std::vector<Row>>
struct AdjacencyMatrix {
    using EdgeLabel = _EdgeLabel;

    AdjacencyMatrix() {}

    AdjacencyMatrix(size_t vertex_num) : m_matrix(vertex_num, Row(vertex_num)) {}

    const size_t vertices_size() const {
        return this->m_matrix.size();
    }

    template<class Func>
    void edges(const Func &func) const {
        REP(i, this->m_matrix.size()) {
            REP(j, this->m_matrix[i].size()) {
                if (!this->m_matrix[i][j]) {
                    continue ;
                }

                if (func(to_edge<EdgeLabel>(i, j, this->m_matrix[i][j]))) {
                    return;
                }
            }
        }
    }

    template<class Func>
    void edges(size_t n1, size_t n2, const Func &func) const {
        if (this->m_matrix[n1][n2]) {
            func(to_edge<EdgeLabel>(n1, n2, this->m_matrix[n1][n2]));
        }
    }

    template<class Func>
    void outgoings(size_t n, const Func &func) const {
        REP(j, this->m_matrix[n].size()) {
            if (!this->m_matrix[n][j]) {
                continue ;
            }
            if (func(to_edge<EdgeLabel>(n, j, this->m_matrix[n][j]))) {
                return;
            }
        }
    }

    auto has_edge(size_t n1, size_t n2) const {
        return static_cast<bool>(this->m_matrix[n1][n2]);
    }

    void add_edge(const Edge<EdgeLabel> &edge) {
        assert(!this->m_matrix[get<0>(edge)][get<1>(edge)]);
        this->m_matrix[get<0>(edge)][get<1>(edge)] = to_element<EdgeLabel>(edge);
    }

    void remove_edge(const Edge<EdgeLabel> &edge) {
        auto elem = to_element<EdgeLabel>(edge);
        if (this->m_matrix[get<0>(edge)][get<1>(edge)] == elem) {
            this->m_matrix[get<0>(edge)][get<1>(edge)] = Element();
        }
    }

    void remove_edge(size_t n1, size_t n2) {
        this->m_matrix[n1][n2] = Element();
    }

    size_t add_vertex() {
        auto n = this->m_matrix.size();
        REP (i, this->m_matrix.size()) {
            this->m_matrix[i].resize(n + 1);
        }
        this->m_matrix.push(Row(n + 1));
        return n;
    }

    void remove_vertex(size_t n) {
        REP (i, this->m_matrix.size()) {
            REP (j, this->m_matrix.size()) {
                if (i == n || j == n) {
                    this->m_matrix[i][j] = Element();
                }
            }
        }
    }

    void to_undirected() {
        std::vector<Edge<EdgeLabel>> es;
        this->edges([&es](const auto &edge) {
            auto e = edge;
            std::swap(get<0>(e), get<1>(e));
            es.push_back(e);

            return false;
        });

        FORE (edge, es) {
            this->add_edge(edge);
        }
    }
private:
    Matrix m_matrix;
    template<class EdgeLabel, typename std::enable_if_t<std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static Edge<EdgeLabel> to_edge(size_t n1, size_t n2, bool element) {
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
    static bool to_element(const Edge<EdgeLabel> &edge) {
        return true;
    }
    template<class EdgeLabel, typename std::enable_if_t<!std::is_void<EdgeLabel>::value, nullptr_t> = nullptr>
    static std::experimental::optional<EdgeLabel> to_element(const Edge<EdgeLabel> &edge) {
        return std::experimental::make_optional(get<2>(edge));
    }
};

using SimpleAdjacencyMatrix = AdjacencyMatrix<void>;
using WeightedAdjacencyMatrix = AdjacencyMatrix<i64>;
using UnsignedWeightedAdjacencyMatrix = AdjacencyMatrix<u64>;
