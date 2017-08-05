#include <vector>
#include <unordered_map>
#include <experimental/optional>

namespace graph {

    struct EdgeSet {
        EdgeSet(const V<pair<size_t, size_t>> &edges) : m_edges(edges) {}

        template<class Iterator>
        EdgeSet(const Iterator &begin, const Iterator &end) : m_edges(begin, end) {}

        const size_t vertices_size() const {
            if (this->m_edges.size() == 0) {
                return 0;
            }

            size_t max = 0;
            FORE(edge, this->m_edges) {
                max = std::max(max, std::max(edge.first, edge.second));
            }
            return max + 1;
        }

        const V<pair<size_t, size_t>> &edges() const {
            return this->m_edges;
        };

        bool has_edge(size_t n1, size_t n2) const {
            return find_if(this->m_edges.begin(), this->m_edges.end(), [n1, n2](auto edge) {
                return edge.first == n1 && edge.second == n2;
            }) != this->m_edges.end();
        }

        V<size_t> incomings(size_t n) const {
            V<size_t> retval;
            FORE(edge, this->m_edges) {
                if (edge.second == n) {
                    retval.push_back(edge.first);
                }
            }
            return retval;
        }

        template<class Pred>
        V <size_t> incomings(size_t n, Pred &&pred) const {
            V<size_t> retval;
            FORE(edge, this->m_edges) {
                if (edge.second == n && pred(edge)) {
                    retval.push_back(edge.first);
                }
            }
            return retval;
        }

        V <size_t> outgoings(size_t n) const {
            V<size_t> retval;
            FORE(edge, this->m_edges) {
                if (edge.first == n) {
                    retval.push_back(edge.second);
                }
            }
            return retval;
        }

        template<class Pred>
        V <size_t> outgoings(size_t n, Pred &&pred) const {
            V<size_t> retval;
            FORE(edge, this->m_edges) {
                if (edge.first == n && pred(edge)) {
                    retval.push_back(edge.second);
                }
            }
            return retval;
        }

        void to_undirected() {
            auto len = this->m_edges.size();
            REP (i, len) {
                auto edge = this->m_edges[i];
                this->m_edges.push_back({edge.second, edge.first});
            }
        }

        void add_edge(size_t n1, size_t n2) {
            this->m_edges.push_back({n1, n2});
        }

        size_t add_vertex() {
            // TODO
            throw runtime_error("Not Implemented");
        }

        void remove_edge(size_t n1, size_t n2) {
            this->m_edges.erase(remove_if(this->m_edges.begin(), this->m_edges.end(), [n1, n2](auto edge) {
                return edge.first == n1 && edge.second == n2;
            }), this->m_edges.end());
        }

        void remove_vertex(size_t n) {
            this->m_edges.erase(remove_if(this->m_edges.begin(), this->m_edges.end(), [n](auto edge) {
                return edge.first == n || edge.second == n;
            }), this->m_edges.end());
        }
    private:
        V<pair<size_t, size_t>> m_edges;
    };

    struct AdjacencyList {
        AdjacencyList(const V<pair<size_t, size_t>> &edges);
        AdjacencyList(const V<V<size_t>> &lists) : m_lists(lists) {}

        const size_t vertices_size() const {
            return this->m_lists.size();
        }

        V<pair<size_t, size_t>> edges() const {
            V<pair<size_t, size_t>> edges;
            REP(i, this->m_lists.size()) {
                const auto &list = this->m_lists[i];
                FORE(n, list) {
                    edges.push_back(make_pair(i, n));
                }
            }
            return edges;
        };

        bool has_edge(size_t n1, size_t n2) const {
            return find(this->m_lists[n1].begin(), this->m_lists[n1].end(), n2) != this->m_lists[n1].end();
        }

        const V<size_t> &incomings(size_t n) const {

        }

        template<class Pred>
        V<size_t> incomings(size_t n, Pred &&pred) const {

        }

        const V<size_t> &outgoings(size_t n) const {
            return this->m_lists[n];
        }

        template<class Pred>
        V <size_t> outgoings(size_t n, Pred &&pred) const {
            V<size_t> retval;
            FORE(n2, this->m_lists[n]) {
                if (pred(make_pair(n, n2))) {
                    retval.push_back(n2);
                }
            }
            return retval;
        }

        void to_undirected() {
            REP(i, this->m_lists.size()) {
                FORE(n, this->m_lists[i]) {

                }
            }
        }

        void add_edge(size_t n1, size_t n2) {

        }

        size_t add_vertex() {
            this->m_lists.push_back({});
            return this->m_lists.size() - 1;
        }

        void remove_edge(size_t n1, size_t n2) {
            this->m_lists[n1].erase(
                    find(this->m_lists[n1].begin(), this->m_lists[n1].end(), n2),
                    this->m_lists[n1].end()
            );
        }

        void remove_vertex(size_t n) {

        }

    private:
        V <V<size_t>> m_lists;
    };
}
