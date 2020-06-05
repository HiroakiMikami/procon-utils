#ifndef MAIN
#include "common.cc"
#endif

static i64 mod(i64 a, i64 m) {
    return (a % m + m) % m;
}

/*
 * n^r
 */
template<class V>
static V pow(V n, i64 r) {
    if (r == 0) {
        return 1;
    }

    auto r2 = r / 2;
    auto x2 = pow(n, r2);

    return x2 * x2 * ((r % 2 == 0) ? 1 : n);
}

static i64 gcd(i64 a, i64 b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

template<class Iterator>
static i64 gcd_ctr(const Iterator &begin, const Iterator &end) {
    if (begin == end) {
        return -1;
    } else {
        auto ans = *begin;
        auto it = begin;
        ++it;
        for (; it != end; ++it) {
            auto x = *it;
            ans = gcd(ans, x);
        }
        return ans;
    }
}

static i64 gcd_ctr(const Vector<i64> &xs) {
    return gcd_ctr(xs.begin(), xs.end());
}



/*
 * a * get<0>(r) + b * get<1>(r) = get<2>(r), get<2>(r) = gcd(a, b)
 */
static tuple<i64, i64, i64> ext_gcd(i64 a, i64 b) {
    auto ext_gcd_ = [](i64 a, i64 b, i64& p, i64 &q, auto f) -> i64 {
        if (b == 0) {
            p = 1;
            q = 0;
            return a;
        }
        i64 d = f(b, a % b, q, p, f);
        q -= a / b * p;
        return d;
    };
    i64 p = 0, q = 0;
    auto d = ext_gcd_(a, b, p, q, ext_gcd_);
    return make_tuple(p, q, d);
}

static i64 lcm(i64 a, i64 b) {
    auto x = gcd(a, b);
    return a / x * b;
}

template<class Iterator>
static i64 lcm_ctr(const Iterator &begin, const Iterator &end) {
    if (begin == end) {
        return -1;
    } else {
        auto ans = *begin;
        auto it = begin;
        ++it;
        for (; it != end; ++it) {
            auto x = *it;
            ans = lcm(ans, x);
        }
        return ans;
    }
}

static i64 lcm_ctr(const Vector<i64> &xs) {
    return lcm_ctr(xs.begin(), xs.end());
}

template<class V>
static V combination(V n, i64 r) {
    if (r == 0) {
        return 1;
    }

    V x = 1;
    FOR (d, 1, r + 1) {
        x *= n;
        n -= 1;
        x /= d;
    }
    return x;
}

static bool is_prime(i64 n) {
    if (n <= 1) return false;
    for (i64 i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

static Vector<i64> divisor(i64 n) {
    Vector<i64> res;
    for (i64 i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            res.push_back(i);
            if (i != n / i) res.push_back(n / i);
        }
    }
    return res;
}

static HashMap<i64, size_t> prime_factor(i64 n) {
    HashMap<i64, size_t> res;
    for (i64 i = 2; i * i <= n; i++) {
        while (n % i == 0) {
            res[i] += 1;
            n /= i;
        }
    }
    if (n != 1) res[n] = 1;
    return res;
}

static pair<Vector<i64>, Vector<bool>> sieve(i64 n) {
    Vector<i64> prime;
    Vector<bool> is_prime_(n + 1, true);
    is_prime_[0] = is_prime_[1] = false;

    FOR (i, 2, n + 1) {
        if (is_prime_[i]) {
            prime.push_back(i);
            for (i64 j = 2 * i; j <= n; j += i) {
                is_prime_[j] = false;
            }
        }
    }
    return {prime, is_prime_};
}

/*
 * x = b1 (mod m1)
 * x = b2 (mod m2)
 * => x = r.first (mod r.second) (r is a return value)
 */
static std::optional<pair<i64, i64>> chinese_rem(i64 b1, i64 m1, i64 b2, i64 m2) {
    auto elem = ext_gcd(m1, m2);
    auto p = get<0>(elem);
    auto d = get<2>(elem);
    if ((b2 - b1) % d != 0) return {};

    i64 m = m1 * (m2 / d); //< lcm(m1, m2)
    i64 r = mod(b1 + m1 * ((b2 - b1) / d * p % (m2 / d)), m);
    return make_optional(std::make_pair(r, m));

}
template <typename Iterator1, typename Iterator2>
static std::optional<pair<i64, i64>> chinese_rem_ctr(Iterator1 b_begin, Iterator1 b_end, Iterator2 m_begin, Iterator2 m_end) {
    i64 r = 0, M = 1;
    auto b = b_begin;
    auto m = m_begin;
    for (; b != b_end && m != m_end; ++b, ++m) {
        auto elem = ext_gcd(M, *m);
        auto p = get<0>(elem);
        auto d = get<2>(elem);

        if ((*b - r) % d != 0) return {};
        r += M * ((*b - r) / d * p % (*m / d));
        M *= *m / d;
    }
    return make_optional(std::make_pair(mod(r, M), M));
}
static std::optional<pair<i64, i64>> chinese_rem_ctr(const Vector<i64> &b, const Vector<i64> &m) {
    return chinese_rem_ctr<decltype(b.begin()), decltype(m.begin())>(CTR(b), CTR(m));
}
