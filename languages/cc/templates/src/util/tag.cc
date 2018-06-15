#include <unordered_map>
#include <vector>
#include <type_traits>
#ifndef MAIN
#include "common.cc"
#endif

template <class V>
struct TagMapping {
    TagMapping(HashMap<V, size_t> map1, vector<V> map2) : m_map1(map1), m_map2(map2) {}

    const V& operator[](const size_t tag) {
        return this->m_map2[tag];
    }
    size_t operator[](const V &value) {
        return this->m_map1.at(value);
    }
private:
    HashMap<V, size_t> m_map1;
    Vector<V> m_map2;
};

template <class Iterator>
auto make_tags(const Iterator& begin, const Iterator &end, size_t size = 0) {
    using V_ = typename std::remove_reference<decltype(*begin)>::type;
    using V = typename std::remove_cv<V_>::type;

    size_t id = 0;
    HashMap<V, size_t> x1;
    vector<V> x2;
    x2.reserve(size);
    for (auto it = begin; it != end; ++it) {
        auto &value = *it;
        x1.insert({value, id});
        x2.push_back(value);

        id += 1;
    }

    return TagMapping<V>(x1, x2);
}
template <class Container>
auto make_tags(const Container &container) {
    return make_tags<decltype(container.begin())>(container.begin(), container.end(), static_cast<size_t>(container.size()));
}
