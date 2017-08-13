#ifndef MAIN
#include "common.cc"
#endif

namespace math {
    /*
     * n^r
     */
    template <class V>
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

    template <class Iterator>
    static i64 gcd_ctr(const Iterator& begin, const Iterator &end) {
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

    static i64 least_common_multiple(i64 a, i64 b) {
        auto x = gcd(a, b);
        return a / x * b;
    }

    template <class Iterator>
    static i64 least_common_multiple_ctr(const Iterator &begin, const Iterator &end) {
        if (begin == end) {
            return -1;
        } else {
            auto ans = *begin;
            auto it = begin;
            ++it;
            for (; it != end; ++it) {
                auto x = *it;
                ans = least_common_multiple(ans, x);
            }
            return ans;
        }
    }
    static i64 least_common_multiple_ctr(const V<i64> &xs) {
        return least_common_multiple_ctr(xs.begin(), xs.end());
    }

    template <class V>
    static V combination(V n, i64 r) {
        if (r == 0) {
            return 1;
        }

        V  x = 1;
        FOR (d, 1, r + 1) {
            x *= n;
            n -= 1;
            x /= d;
        }
        return x;
    }
}
