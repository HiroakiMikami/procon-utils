#ifndef MAIN
#include "common.cc"
#endif

template <class T, class Compare = std::less<T>>
struct Heap {
    Heap () : m_heap(), m_cmp() {}
    Heap (const Compare &cmp) : m_heap(), m_cmp(cmp) {}

    void reserve(const size_t size) {
        this->m_heap.reserve(size);
    }

    const T &top() const {
        return this->m_heap.front();
    }
    T pop() {
        pop_heap(this->m_heap.begin(), this->m_heap.end(), this->m_cmp);
        auto retval = this->m_heap.back();
        this->m_heap.pop_back();
        return retval;
    }
    void push(const T &v) {
        this->m_heap.push_back(v);
        push_heap(this->m_heap.begin(), this->m_heap.end(), this->m_cmp);
    }

    size_t size() const {
        return this->m_heap.size();
    }

    auto begin() const {
        return this->m_heap.cbegin();
    }
    auto cbegin() const {
        return this->m_heap.cbegin();
    }
    auto end() const {
        return this->m_heap.cend();
    }
    auto cend() const {
        return this->m_heap.cend();
    }
private:
    V<T> m_heap;
    Compare m_cmp;
};
