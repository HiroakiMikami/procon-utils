#ifndef MAIN
#include "common.cc"
#endif

template <class T>
struct BinaryIndexedTree {
    BinaryIndexedTree() : m_bit(0, 0) {}
    BinaryIndexedTree(const size_t size) : m_bit(size, 0) {}
    BinaryIndexedTree(const vector<T> &vector) : m_bit(vector) {
        REP (i, this->m_bit.size()) {
            auto j = i + lsb(i + 1);
            if (j < this->m_bit.size()) {
                this->m_bit[j] += this->m_bit[i];
            }
        }
    }
    template <class Iterator>
    BinaryIndexedTree(const Iterator &begin, const Iterator &end) : m_bit(distance(begin, end), 0) {
        auto it = begin;
        REP (i, this->m_bit.size()) {
            this->m_bit[i] = *it;
            auto j = i + lsb(i + 1);
            if (j < this->m_bit.size()) {
                this->m_bit[j] += this->m_bit[i];
            }

            ++it;
        }
    }

    void add(size_t index, const T& delta) {
        while (index < this->size()) {
            this->m_bit[index] += delta;
            index += lsb(index + 1);
        }
    }
    auto sum(size_t length) const {
        size_t index = length;
        auto retval = T();
        while (index != 0) {
            retval += this->m_bit[index - 1];
            index -= lsb(index);
        }
        return retval;
    }
    auto sum(size_t begin, size_t end) const {
        // sum between begin ... end-1
        return this->sum(end) - this->sum(begin);
    }
    auto at(size_t index) const {
        return this->sum(index, index + 1);
    }

    size_t size() const {
        return this->m_bit.size();
    }

    auto begin() const {
        return this->m_bit.cbegin();
    }
    auto cbegin() const {
        return this->m_bit.cbegin();
    }
    auto end() const {
        return this->m_bit.cend();
    }
    auto cend() const {
        return this->m_bit.cend();
    }
private:
    static auto lsb(size_t i) {
        return i & -i;
    }
    vector<T> m_bit;
};
template <class T> using BIT = BinaryIndexedTree<T>;

namespace debug {
    template <typename T>
    struct oneline<BIT<T>> {
        std::string operator()(const BIT<T> &t) const {
            Vector<T> xs;
            REP (i, t.size()) {
                xs.push_back(t.at(i));
            }
            return oneline<decltype(xs)>()(xs);
        }
    };
}
