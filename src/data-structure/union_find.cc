template <class T>
struct union_find {
    union_find(const vector<T> &elems) {
        this->m_parents.reserve(elems.size());
        for (const auto &elem: elems) {
            m_parents.insert({elem, elem});
            m_rank.insert({elem, 0});
        }
    }
    template <class Iterator>
    union_find(const Iterator& begin, const Iterator& end) {
        for (auto it = begin; it != end; it++) {
            m_parents.insert({*it, *it});
            m_rank.insert({*it, 0});
        }
    }

    void merge(const T& t1, const T& t2) {
        const auto& p1 = this->parent(t1);
        const auto& p2 = this->parent(t2);

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
    bool is_same(const T& t1, const T& t2) const {
        return this->parent(t1) == this->parent(t2);
    }

    const T &parent(const T& t) const {
        const auto& p = this->m_parents[t];
        if (p == t) {
            return t;
        } else {
            const auto& p2 = this->parent(p);
            this->m_parents[t] = p2;
            return p2;
        }
    }
private:
    mutable unordered_map<T, T> m_parents;
    unordered_map<T, u64> m_rank;
};

template <>
struct union_find<size_t> {
    union_find(size_t N) : m_parents(N), m_rank(N) {
        for (auto i = 0; i < N; i++) {
            m_parents[i] = i;
            m_rank[i] = 0;
        }
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