#ifndef MAIN
#include "common.cc"
#include "math.cc"
#endif

namespace mod_integer {
    const i64 MOD = 1000000007;
}

template <i64 M = mod_integer::MOD>
struct ModInteger {
    ModInteger() : value(0) {}
    ModInteger(i64 value) {
        update_value((value));
    }

    ModInteger inverse() const {
        return pow(*this, M - 2); // TODO 原理はよくわからない
    }

    ModInteger operator+(const ModInteger &rhs) const {
        return this->value + rhs.value;
    }
    ModInteger operator-(const ModInteger &rhs) const {
        return this->value - rhs.value;
    }

    ModInteger operator*(const ModInteger &rhs) const {
        return this->value * rhs.value;
    }

    ModInteger operator /(const ModInteger &rhs) const {
        return this->value * rhs.inverse().value;
    }

    void operator +=(const ModInteger &rhs) {
        update_value(this->value + rhs.value);
    }

    void operator -=(const ModInteger &rhs) {
        update_value(this->value - rhs.value);
    }

    void operator *=(const ModInteger &rhs) {
        update_value(this->value * rhs.value);
    }

    void operator /=(const ModInteger &rhs) {
        update_value(this->value * rhs.inverse().value);
    }

    bool operator==(const ModInteger &rhs) const {
        return this->value == rhs.value;
    }
    template <class V>
    bool operator==(V rhs) const {
        return this->value == rhs;
    }

    bool operator!=(const ModInteger &rhs) const {
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
static bool operator==(V lhs, const ModInteger<M> &rhs) {
    return rhs ==lhs;
}
template <class V, i64 M>
static bool operator!=(V lhs, const ModInteger<M> &rhs) {
    return rhs != lhs;
}

namespace mod_integer {
    template <i64 M>
    struct fact {
        ModInteger<M> value;
        ModInteger<M> inverse;
    };

    template <i64 M>
    static Vector<fact<M>> fact_table(const size_t n) {
        Vector<fact<M>> retval(n + 1);

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
}

namespace internal {
    template <i64 M>
    struct oneline<ModInteger<M>> {
        std::string operator()(const ModInteger<M> &x) const {
            return oneline<decltype(x.get())>()(x.get()) + "(mod " + oneline<decltype(M)>()(M) + ")";
        }
    };
}
