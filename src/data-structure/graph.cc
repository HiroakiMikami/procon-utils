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
        class EntryList = vector<typename conditional<is_void<_EdgeLabel>::value, tuple<size_t>, tuple<size_t, _EdgeLabel>>::type>,
        class List = vector<EntryList>>
struct AdjacencyList {
    using EdgeLabel = _EdgeLabel;

    struct EdgeIteratorFunctions {
        EdgeIteratorFunctions(size_t to): to(to) {}
        EdgeIteratorFunctions(): to() {}
        using State = std::tuple<size_t, size_t, const AdjacencyList *>;
        bool is_begin(const State &state) const {
            return get<0>(state) == 0 && get<1>(state) == 0;
        }
        bool is_end(const State &state) const {
            return get<0>(state) == get<2>(state)->m_list.size();
        }
        bool is_valid(const State &state) const {
            auto i = get<0>(state);
            auto j = get<1>(state);

            if (get<2>(state)->m_list.size() == i) {
                return true;
            }
            if (get<2>(state)->m_list[i].size() == j) {
                return false;
            }

            if (this->to) {
                return get<0>(get<2>(state)->m_list[i][j]) == this->to.value();
            } else {
                return true;
            }
        }
        Edge<EdgeLabel> get_value(const State &state) const {
            return to_edge(get<0>(state), get<2>(state)->m_list[get<0>(state)][get<1>(state)]);
        }

        Edge<EdgeLabel> get_value(State &state) {
            return to_edge(get<0>(state), get<2>(state)->m_list[get<0>(state)][get<1>(state)]);
        }

        void next(State &state) const {
            if ((get<2>(state)->m_list[get<0>(state)].size() == 0) ||
                (get<1>(state) + 1) == get<2>(state)->m_list[get<0>(state)].size()) {
                get<0>(state) += 1;
                get<1>(state) = 0;
            } else {
                get<1>(state) += 1;
            }
        }
        void previous(State &state) const {
            if (get<1>(state) == 0) {
                get<0>(state) -= 1;
                get<1>(state) = get<2>(state)->m_list[get<0>(state)].size() - 1;
            } else {
                get<1>(state) -= 1;
            }
        }
    private:
        std::experimental::optional<size_t> to;
    };
    using EdgeIterator = BaseIterator<EdgeIteratorFunctions>;

    AdjacencyList() {}
    AdjacencyList(size_t vertex_num) : m_list(vertex_num) {}

    const size_t vertices_size() const {
        return this->m_list.size();
    }

    Container<EdgeIterator> edges() const {
        auto begin = EdgeIterator(std::make_tuple(0, static_cast<size_t>(0), this));
        auto end = EdgeIterator(std::make_tuple(this->m_list.size(), static_cast<size_t>(0), this));
        return Container<EdgeIterator>(begin, end);
    }
    Container<EdgeIterator> edges(size_t n1, size_t n2) const {
        auto begin = EdgeIterator(std::make_tuple(n1, static_cast<size_t>(0), this), EdgeIteratorFunctions(n2));
        auto end = EdgeIterator(std::make_tuple(n1 + 1, static_cast<size_t>(0), this), EdgeIteratorFunctions(n2));
        return Container<EdgeIterator>(begin, end);
    }

    Container<EdgeIterator> outgoings(size_t n) const {
        auto begin = EdgeIterator(std::make_tuple(n, static_cast<size_t>(0), this));
        auto end = EdgeIterator(std::make_tuple(n + 1, static_cast<size_t>(0), this));
        return Container<EdgeIterator>(begin, end);
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
        for (auto edge: this->edges()) {
            std::swap(get<0>(edge), get<1>(edge));
            es.push_back(edge);
        }

        FORE (edge, es) {
            this->add_edge(edge);
        }
    }
private:
    List m_list;

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

    struct EdgeIteratorFunctions {
        EdgeIteratorFunctions() {}
        using State = std::tuple<size_t, size_t, const AdjacencyMatrix *>;
        bool is_begin(const State &state) const {
            return get<0>(state) == 0 && get<1>(state) == 0;
        }
        bool is_end(const State &state) const {
            return get<0>(state) == get<2>(state)->m_matrix.size();
        }
        bool is_valid(const State &state) const {
            return static_cast<bool>(get<2>(state)->m_matrix[get<0>(state)][get<1>(state)]);
        }
        Edge<EdgeLabel> get_value(const State &state) const {
            return to_edge<EdgeLabel>(get<0>(state), get<1>(state), get<2>(state)->m_matrix[get<0>(state)][get<1>(state)]);
        }

        Edge<EdgeLabel> get_value(State &state) {
            return to_edge<EdgeLabel>(get<0>(state), get<1>(state), get<2>(state)->m_matrix[get<0>(state)][get<1>(state)]);
        }

        void next(State &state) const {
            if ((get<2>(state)->m_matrix[get<0>(state)].size() == 0) ||
                (get<1>(state) + 1) == get<2>(state)->m_matrix[get<0>(state)].size()) {
                get<0>(state) += 1;
                get<1>(state) = 0;
            } else {
                get<1>(state) += 1;
            }
        }
        void previous(State &state) const {
            if (get<1>(state) == 0) {
                get<0>(state) -= 1;
                get<1>(state) = get<2>(state)->m_matrix[get<0>(state)].size() - 1;
            } else {
                get<1>(state) -= 1;
            }
        }
    };
    using EdgeIterator = BaseIterator<EdgeIteratorFunctions>;

    AdjacencyMatrix() {}

    AdjacencyMatrix(size_t vertex_num) : m_matrix(vertex_num, Row(vertex_num)) {}

    const size_t vertices_size() const {
        return this->m_matrix.size();
    }

    Container<EdgeIterator> edges() const {
        auto begin = EdgeIterator(std::make_tuple(0, static_cast<size_t>(0), this));
        auto end = EdgeIterator(std::make_tuple(this->m_matrix.size(), static_cast<size_t>(0), this));
        return Container<EdgeIterator>(begin, end);
    }
    Container<EdgeIterator> edges(size_t n1, size_t n2) const {
        auto begin = EdgeIterator(std::make_tuple(n1, n2, this));
        auto end = EdgeIterator(std::make_tuple(n1, n2 + 1, this));
        return Container<EdgeIterator>(begin, end);
    }

    Container<EdgeIterator> outgoings(size_t n) const {
        auto begin = EdgeIterator(std::make_tuple(n, static_cast<size_t>(0), this));
        auto end = EdgeIterator(std::make_tuple(n + 1, static_cast<size_t>(0), this));
        return Container<EdgeIterator>(begin, end);
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
        for (auto edge: this->edges()) {
            std::swap(get<0>(edge), get<1>(edge));
            es.push_back(edge);
        }

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
