#ifndef MAIN
#include "common.cc"
#endif

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

static i64 gcd_ctr(const V<i64> &xs) {
    return gcd_ctr(xs.begin(), xs.end());
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

static i64 lcm_ctr(const V<i64> &xs) {
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

static V<i64> divisor(i64 n) {
    V<i64> res;
    for (i64 i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            res.push_back(i);
            if (i != n / i) res.push_back(n / i);
        }
    }
    return res;
}

static unordered_map<i64, size_t> prime_factor(i64 n) {
    unordered_map<i64, size_t> res;
    for (i64 i = 2; i * i <= n; i++) {
        while (n % i == 0) {
            res[i] += 1;
            n /= i;
        }
    }
    if (n != 1) res[n] = 1;
    return res;
}

static pair<V<i64>, V<bool>> sieve(i64 n) {
    V<i64> prime;
    V<bool> is_prime_(n + 1, true);
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
