#include <iostream>
#include <cstdint>
#include <utility>
#include <tuple>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <algorithm>
#include <limits>
#include <numeric>
#include <iomanip>
#include <type_traits>
#include <functional>

using namespace std; // TODO Remove this

/* macros */
// loops
#define REP(i, n) for (i64 i = 0; i < static_cast<decltype(i)>(n); ++i)
#define REPR(i, n) for (i64 i = (n) - 1; i >= static_cast<decltype(i)>(0); --i)
#define FOR(i, n, m) for (i64 i = (n); i < static_cast<decltype(i)>(m); ++i)
#define FORR(i, n, m) for (i64 i = (m) - 1; i >= static_cast<decltype(i)>(n); --i)
#define EACH(x, xs) for (auto &x: (xs))
#define EACH_V(x, xs) for (auto x: (xs))

// helpers
#define CTR(x) (x).begin(), (x).end()

/* internal code */
namespace internal {
    /* utils for std::tuple */
    namespace tuple_utils {
        template<size_t...>
        struct seq {};

        template<size_t N, size_t... Is>
        struct gen_seq : gen_seq<N - 1, N - 1, Is...> {};

        template<size_t... Is>
        struct gen_seq<0, Is...> : seq<Is...> {};

        template<class Tuple, size_t... Is>
        void read(istream &stream, Tuple &t, seq<Is...>) {
            static_cast<void>((int[]) {0, (void(stream >> get<Is>(t)), 0)...});
        }

        template<class Tuple, size_t... Is>
        void print(ostream &stream, Tuple const &t, seq<Is...>) {
            static_cast<void>((int[]) {0, (void(stream << (Is == 0 ? "" : ", ") << get<Is>(t)), 0)...});
        }

        template<size_t I, class F, class A, class... Elems>
        struct ForEach {
            void operator()(A &arg, tuple<Elems...> const &t) const {
                F()(arg, get<I>(t));
                ForEach<I - 1, F, A, Elems...>()(arg, t);
            }

            void operator()(A &arg, tuple<Elems...> &t) const {
                F()(arg, get<I>(t));
                ForEach<I - 1, F, A, Elems...>()(arg, t);
            }
        };

        template<class F, class A, class... Elems>
        struct ForEach<0, F, A, Elems...> {
            void operator()(A &arg, tuple<Elems...> const &t) const {
                F()(arg, get<0>(t));
            }

            void operator()(A &arg, tuple<Elems...> &t) const {
                F()(arg, get<0>(t));
            }
        };

        template<class F, class A, class... Elems>
        void for_each(A &arg, tuple<Elems...> const &t) {
            ForEach<tuple_size<tuple<Elems...>>::value - 1, F, A, Elems...>()(arg, t);
        }

        template<class F, class A, class... Elems>
        void for_each(A &arg, tuple<Elems...> &t) {
            ForEach<tuple_size<tuple<Elems...>>::value - 1, F, A, Elems...>()(arg, t);
        }

        struct hash_for_element {
            template<class V>
            void operator()(size_t &size, const V &v) const {
                size ^= hash<V>()(v);
            }
        };
    }

    /* utils for std::vector */
    namespace vector_utils {
        template <typename V, int N>
        struct matrix_t {
            using type = std::vector<typename matrix_t<V, N-1>::type>;
        };
        template <typename V>
        struct matrix_t<V, 0> {
            using type = V;
        };

        template <typename V, int N>
        using Matrix = typename internal::vector_utils::matrix_t<V, N>::type;

        template <typename V, typename It, int N>
        struct matrix_helper {
            static Matrix<V, N> create(const It &begin, const It &end, const V &default_value) {
                return Matrix<V, N>(*begin, matrix_helper<V, It, N - 1>::create(begin + 1, end, default_value));
            }
        };
        template <typename V, typename It>
        struct matrix_helper<V, It, 0> {
            static Matrix<V, 0> create(const It &begin, const It &end, const V &default_value) {
                return default_value;
            }
        };
    }
}


/* Primitive types */
using i8  =  int8_t; using  u8 =  uint8_t;
using i16 = int16_t; using u16 = uint16_t;
using i32 = int32_t; using u32 = uint32_t;
using i64 = int64_t; using u64 = uint64_t;
using usize = size_t;

/* Data structure type */
template <class T> using V = vector<T>; // TODO Replace with Vector
template <typename T> using Vector = internal::vector_utils::Matrix<T, 1>;
template <typename T, int N> using Matrix = internal::vector_utils::Matrix<T, N>;
template <typename V> using OrderedSet = std::set<V>;
template <typename V> using HashSet = std::unordered_set<V>;
template <typename K, typename V> using OrderedMap = std::map<K, V>;
template <typename K, typename V> using HashMap = std::unordered_map<K, V>;

/* utils for std::vector */
template <typename V>
std::vector<V> make_pre_allocated_vector(size_t N) {
    std::vector<V> retval;
    retval.reserve(N);
    return retval;
}

template <class V, int N>
Matrix<V, N> make_matrix(const std::array<size_t, N> &shape, V default_value = V()) {
    return internal::vector_utils::matrix_helper<V, decltype(shape.begin()), N>::create(shape.begin(), shape.end(), default_value);
}
/* utils for STL containers */
template <class Iterator>
struct Container {
    Container(const Iterator &begin, const Iterator &end) : m_begin(begin), m_end(end) {}
    const Iterator& begin() const {
        return this->m_begin;
    }
    const Iterator& end() const {
        return this->m_end;
    }
    Iterator m_begin;
    Iterator m_end;
};

/* utils for STL iterators */
template <typename Iterator, typename F>
struct MappedIterator {
    MappedIterator(const Iterator &it, const F &function) : it(it), function(function) {}
    auto operator *() const {
        return this->function(this->it);
    }
    void operator++() { ++this->it; }
    void operator+=(size_t n) { this->it += n; }
    auto operator+(size_t n) const {
        return MappedIterator<Iterator, F>(this->it + n, this->function);
    }
    bool operator==(const MappedIterator<Iterator, F> &rhs) const {
        return this->it == rhs.it;
    }
    bool operator!=(const MappedIterator<Iterator, F> &rhs) const {
        return !(*this == rhs);
    }
private:
    Iterator it;
    F function;
};
template <typename Iterator, typename P>
struct FilteredIterator {
    FilteredIterator(const Iterator &it, const Iterator &end, const P &predicate)
            : it(it), end(end), predicate(predicate) {
        if (this->it != end) {
            if (!predicate(this->it)) {
                this->increment();
            }
        }
    }
    decltype(auto) operator *() const {
        return *this->it;
    }
    auto operator ->() const {
        return this->it;
    }
    void operator++() {
        this->increment();
    }
    void operator+=(size_t n) {
        REP (i, n) {
            this->increment();
        }
    }
    auto operator+(size_t n) const {
        auto retval = *this;
        retval += n;
        return retval;
    }
    bool operator==(const FilteredIterator<Iterator, P> &rhs) const {
        return this->it == rhs.it;
    }
    bool operator!=(const FilteredIterator<Iterator, P> &rhs) const {
        return !(*this == rhs);
    }
private:
    void increment() {
        if (this->it == this->end) {
            return ;
        }
        ++this->it;
        while (this->it != this->end && !this->predicate(this->it)) {
            ++this->it;
        }
    }
    Iterator it;
    Iterator end;
    P predicate;
};
template <typename Iterator, typename ElementIterator>
struct FlattenedIterator {
    FlattenedIterator(const Iterator &it, const Iterator &end) : it(make_pair(it, ElementIterator())), end(end) {
        if (this->it.first != this->end) {
            this->it.second = it->begin();
        }
    }
    decltype(auto) operator *() const {
        return this->it;
    }
    const pair<Iterator, ElementIterator> *operator ->() const {
        return &this->it;
    }
    void operator++() {
        this->increment();
    }
    void operator+=(size_t n) {
        REP (i, n) {
            this->increment();
        }
    }
    auto operator+(size_t n) const {
        auto retval = *this;
        retval += n;
        return retval;
    }
    bool operator==(const FlattenedIterator<Iterator, ElementIterator> &rhs) const {
        if (this->it.first != rhs.it.first) {
            return false;
        }
        if (this->it.first == this->end || rhs.it.first == rhs.end) {
            if (this->end == rhs.end) {
                return true;
            } else {
                return false;
            }
        } else {
            return this->it.second == rhs.it.second;
        }
    }
    bool operator!=(const FlattenedIterator<Iterator, ElementIterator> &rhs) const {
        return !(*this == rhs);
    }
private:
    void increment() {
        auto is_end = [this]() {
            return this->it.first == this->end;
        };
        auto is_valid = [this, &is_end]() {
            if (is_end()) return false;
            if (this->it.second == this->it.first->end()) return false;
            return true;
        };

        auto increment = [this]() {
            if (this->it.second == this->it.first->end()) {
                ++this->it.first;
                if (this->it.first != this->end) {
                    this->it.second = this->it.first->begin();
                }
            } else {
                ++this->it.second;
            }
        };

        increment();
        while (!is_end() && !is_valid()) {
            increment();
        }

    }
    pair<Iterator, ElementIterator> it;
    Iterator end;
};

template <typename C, typename F>
auto iterator_map(const C &c, F function) {
    using Iterator = std::remove_const_t<std::remove_reference_t<decltype(c.begin())>>;
    return Container<MappedIterator<Iterator, F>>(MappedIterator<Iterator, F>(c.begin(), function),
            MappedIterator<Iterator, F>(c.end(), function));
}

template <typename C, typename P>
auto iterator_filter(const C &c, P predicate) {
    using Iterator = std::remove_const_t<std::remove_reference_t<decltype(c.begin())>>;
    return Container<FilteredIterator<Iterator, P>>(FilteredIterator<Iterator, P>(c.begin(), c.end(), predicate),
                                                    FilteredIterator<Iterator, P>(c.end(), c.end(), predicate));
}

template <typename C>
auto iterator_flatten(const C &c) {
    using Iterator = std::remove_const_t<std::remove_reference_t<decltype(c.begin())>>;
    using ElementIterator = std::remove_const_t<std::remove_reference_t<decltype(c.begin()->begin())>>;
    return Container<FlattenedIterator<Iterator, ElementIterator>>(
            FlattenedIterator<Iterator, ElementIterator>(c.begin(), c.end()),
            FlattenedIterator<Iterator, ElementIterator>(c.end(), c.end()));
}

/* input */
template <class F, class S>
istream &operator>>(istream &stream, pair<F, S> &pair) {
    stream >> pair.first;
    stream >> pair.second;
    return stream;
}
template <class ...Args>
istream &operator>>(istream &stream, tuple<Args...> &tuple) {
    internal::tuple_utils::read(stream, tuple, internal::tuple_utils::gen_seq<sizeof...(Args)>());
    return stream;
}

template <class T>
T read() {
    T t;
    cin >> t;
    return t;
}
template <class F, class S>
pair<F, S> read() {
    pair<F, S> p;
    cin >> p;
    return p;
}
template <class T1, class T2, class T3, class ...Args>
tuple<T1, T2, T3, Args...> read() {
    tuple<T1, T2, T3, Args...> t;
    cin >> t;
    return t;
}
template <typename T, typename F = std::function<T()>>
Vector<T> read(const usize length, F r) {
    auto retval = make_pre_allocated_vector<T>(length);
    REP (i, length) {
        retval.emplace_back(r());
    }
    return retval;
}

template <class T>
Vector<T> read(const usize length) {
    return read<T>(length, [] { return read<T>(); });
}
template <class F, class S>
Vector<pair<F, S>> read(const usize length) {
    return read<pair<F, S>>(length);
}
template <class T1, class T2, class T3, class ...Args>
Vector<tuple<T1, T2, T3, Args...>> read(const usize length) {
    return read<tuple<T1, T2, T3, Args...>>(length);
}

/* debug output */
template <class F, class S>
ostream &operator<<(ostream& stream, const pair<F, S> &pair) {
    stream << "{" << pair.first << ", " << pair.second << "}";
    return stream;
}
template <class ...Args>
ostream &operator<<(ostream& stream, const tuple<Args...> &tuple) {
    stream << "{";
    internal::tuple_utils::print(stream, tuple, internal::tuple_utils::gen_seq<sizeof...(Args)>());
    stream << "}";
    return stream;
}

template <typename T>
void dump(const T& t) {
    std::cerr << t << std::endl;
}
template <typename F, typename S>
void dump(const pair<F, S>& p) {
    std::cerr << p << std::endl;
}
template <class ...Args>
void dump(const tuple<Args...> &tuple) {
    std::cerr << tuple << std::endl;
}
template <typename V1, typename V2, typename ...Args>
void dump(const V1 &v1, const V2 &v2, const Args&... args) {
    const auto x = std::make_tuple(v1, v2, args...);
    internal::tuple_utils::print(std::cerr, x, internal::tuple_utils::gen_seq<sizeof...(Args) + 2>());
    std::cerr << std::endl;
}

template <typename T>
void dump_matrix(const Matrix<T, 0> & matrix) {
    dump(matrix);
}
template <typename T>
void dump_matrix(const Matrix<T, 1> & matrix) {
    EACH (x, matrix) {
        std::cerr << x << " ";
    }
    std::cerr << std::endl;
}
template <typename T>
void dump_matrix(const Matrix<T, 2> & matrix) {
    EACH (x, matrix) {
        dump_matrix<T>(x);
    }
}

template <typename C>
void dump_set(const C &container) {
    EACH(c, container) {
        dump(c);
    }
}
template <typename C>
void dump_seq(const C &container) {
    int index = 0;
    EACH(c, container) {
        dump(index, c);
        index += 1;
    }
}
template <typename C>
void dump_map(const C &container) {
    EACH(c, container) {
        const auto &key = c.first;
        const auto &value = c.second;
        std::cerr << key << "\t:" << value << std::endl;
    }
}

// Hash
namespace std {
    template <class F, class S>
    struct hash<pair<F, S>> {
        size_t operator ()(const pair<F, S> &p) const {
            return hash<F>()(p.first) ^ hash<S>()(p.second);
        }
    };

    template <class ...Args>
    struct hash<tuple<Args...>> {
        size_t operator ()(const tuple<Args...> &t) const {
            size_t retval = 0;

            internal::tuple_utils::for_each<internal::tuple_utils::hash_for_element, size_t, Args...>(retval, t);

            return retval;
        }
    };
}
