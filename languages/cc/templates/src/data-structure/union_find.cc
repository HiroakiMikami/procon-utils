#ifndef MAIN
#include "common.cc"
#endif

struct UnionFind {
    UnionFind() : m_parents(0), m_rank(0) {}
    UnionFind(size_t N) : m_parents(N), m_rank(N) {
        REP (i, N) {
            m_parents[i] = i;
            m_rank[i] = 0;
        }
    }

    size_t size() const {
        return this->m_rank.size();
    }

    void merge(size_t t1, size_t t2) {
        auto p1 = this->parent(t1);
        auto p2 = this->parent(t2);

        if (p1 == p2) {
            return ;
        }

        if (this->m_rank[p1] < this->m_rank[p2]) {
            this->m_parents[p1] = p2;
        } else {
            this->m_parents[p2] = p1;
            if (this->m_rank[p1] == this->m_rank[p2]) {
                this->m_rank[p1] += 1;
            }
        }
    }
    bool is_same(size_t t1, size_t t2) const {
        return this->parent(t1) == this->parent(t2);
    }

    size_t parent(size_t t) const {
        auto p = this->m_parents[t];
        if (p == t) {
            return t;
        } else {
            auto p2 = this->parent(p);
            this->m_parents[t] = p2;
            return p2;
        }
    }
private:
    mutable vector<size_t> m_parents;
    vector<u64> m_rank;

};

namespace internal {
    template <>
    struct oneline<UnionFind> {
        std::string operator()(const UnionFind &t) const {
            OrderedMap<size_t, OrderedSet<size_t>> xs;
            REP (i, t.size()) {
                xs[t.parent(i)].insert(i);
            }
            return oneline<decltype(xs)>()(xs);
        }
    };
}
