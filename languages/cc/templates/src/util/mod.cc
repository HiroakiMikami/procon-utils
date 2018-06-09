#ifndef MAIN
#include "common.cc"
#include "math.cc"
#endif

namespace mod {
    const i64 MOD = 1000000007;

    template <i64 M>
    struct Integer {
        Integer() : value(0) {}
        Integer(i64 value) {
            update_value((value));
        }

        Integer inverse() const {
            return pow(*this, M - 2); // TODO 原理はよくわからない
        }

        Integer operator+(const Integer &rhs) const {
            return this->value + rhs.value;
        }
        Integer operator-(const Integer &rhs) const {
            return this->value - rhs.value;
        }

        Integer operator*(const Integer &rhs) const {
            return this->value * rhs.value;
        }

        Integer operator /(const Integer &rhs) const {
            return this->value * rhs.inverse().value;
        }

        void operator +=(const Integer &rhs) {
            update_value(this->value + rhs.value);
        }

        void operator -=(const Integer &rhs) {
            update_value(this->value - rhs.value);
        }

        void operator *=(const Integer &rhs) {
            update_value(this->value * rhs.value);
        }

        void operator /=(const Integer &rhs) {
            update_value(this->value * rhs.inverse().value);
        }

        bool operator==(const Integer &rhs) const {
            return this->value == rhs.value;
        }
        template <class V>
        bool operator==(V rhs) const {
            return this->value == rhs;
        }

        bool operator!=(const Integer &rhs) const {
            return this->value != rhs.value;
        }
        template <class V>
        bool operator!=(V rhs) const {
            return this->value != rhs;
        }

        const i64 &get() const {
            return this->value;
        }

    private:
        void update_value(i64 new_value) {
            this->value = new_value;
            if (this->value < 0) {
                this->value += M;
            }
            if (this->value >= M) {
                this->value %= M;
            }
        }
        i64 value;
    };

    template <class V, i64 M>
    static bool operator==(V lhs, const Integer<M> &rhs) {
        return rhs ==lhs;
    }
    template <class V, i64 M>
    static bool operator!=(V lhs, const Integer<M> &rhs) {
        return rhs != lhs;
    }

    template <i64 M>
    struct fact {
        Integer<M> value;
        Integer<M> inverse;
    };

    template <i64 M>
    static V<fact<M>> fact_table(const size_t n) {
        V<fact<M>> retval(n + 1);

        retval[0].value = 1;
        FOR(i, 1, n + 1) {
            retval[i].value = retval[i - 1].value * i;
        }

        retval[n].inverse = retval[n].value.inverse();
        REPR(i, n) {
            retval[i].inverse = retval[i + 1].inverse * (i + 1);
        }

        return retval;
    }

    using Int = Integer<mod::MOD>;
}

namespace debug {
    template <i64 M>
    struct oneline<mod::Integer<M>> {
        std::string operator()(const mod::Integer<M> &x) const {
            return oneline<decltype(x.get())>()(x.get()) + "(mod " + oneline<decltype(M)>()(M) + ")";
        }
    };
}
