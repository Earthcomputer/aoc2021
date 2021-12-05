#pragma once

#include <sstream>
#include <functional>

namespace util {
    std::vector<std::string> split(const std::string& input, const std::string& sep);

    template<class T>
    T convert(const std::string& str) {
        std::stringstream ss(str);
        T result;
        ss >> result;
        return result;
    }

    template<class T>
    constexpr void minmax(T& a, T& b) {
        if (b < a) {
            std::swap(a, b);
        }
    }

    template<class T>
    constexpr int bits_required(T value) {
        if (value <= 0) {
            return 0;
        }
        int bits = 0;
        for (T res = 1;; res <<= 1) {
            if (res > value) {
                return bits;
            }
            ++bits;
        }
    }
}
