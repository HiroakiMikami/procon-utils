#include <type_traits>

#ifndef MAIN
#include "common.cc"
#endif

template <class T>
T to(const string &str) {
}

template <>
i64 to(const string &str) {
    return atoll(str.c_str());
}

template <>
i32 to(const string &str) {
    return atoi(str.c_str());
}

V<string> split(const string &str, const string &delimiter) {
    V<string> ret;
    std::remove_const_t<decltype(string::npos)> pos = 0;
    while (pos != string::npos) {
        auto p = str.find(delimiter, pos);
        if (p == string::npos) {
            ret.push_back(str.substr(pos));
            break ;
        } else {
            ret.push_back(str.substr(pos, p - pos));
        }
        pos = p + 1;
    }

    return ret;
}