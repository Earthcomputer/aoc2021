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
    void minmax(T& a, T& b) {
        if (b < a) {
            std::swap(a, b);
        }
    }
}
