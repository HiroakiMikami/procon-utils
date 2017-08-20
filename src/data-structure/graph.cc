#include <vector>
#include <unordered_map>
#include <experimental/optional>
#ifndef MAIN
#include "common.cc"
#endif

namespace graph {
    struct AdjacencyList {
        AdjacencyList() {}
        AdjacencyList(size_t vertex_num) : m_lists(vertex_num) {}

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

        V<size_t> incomings(size_t n) const {
            V<size_t> retval;
            REP (i, this->m_lists.size()) {
                FORE(e, this->m_lists[i]) {
                    if (e == n) {
                        retval.push_back(i);
                    }
                }
            }
            return retval;
        }

        template<class Pred>
        V<size_t> incomings(size_t n, Pred &&pred) const {
            V<size_t> retval;
            REP (i, this->m_lists.size()) {
                FORE(e, this->m_lists[i]) {
                    if (e == n && pred(make_pair(i, n))) {
                        retval.push_back(i);
                    }
                }
            }
            return retval;
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
            auto ls = this->m_lists;
            REP(i, ls.size()) {
                FORE(n, ls[i]) {
                    this->m_lists[n].push_back(i);
                }
            }
        }

        void add_edge(size_t n1, size_t n2) {
            this->m_lists[n1].push_back(n2);
        }

        size_t add_vertex() {
            this->m_lists.push_back({});
            return this->m_lists.size() - 1;
        }

        void remove_edge(size_t n1, size_t n2) {
            this->m_lists[n1].erase(
                    remove(this->m_lists[n1].begin(), this->m_lists[n1].end(), n2),
                    this->m_lists[n1].end()
            );
        }

        void remove_vertex(size_t n) {
            for (auto i = 0; i < this->m_lists.size(); ++i) {
                if (i == n) {
                    this->m_lists[i].clear();
                } else {
                    auto &edges = this->m_lists[i];
                    edges.erase(remove(edges.begin(), edges.end(), n), edges.end());
                }
            }
        }

    private:
        V <V<size_t>> m_lists;
    };
}
