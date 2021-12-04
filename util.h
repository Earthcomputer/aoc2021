#pragma once

#include <sstream>
#include <functional>

namespace util {
    std::vector<std::string> split(const std::string& input, const std::string& sep) {
        std::vector<std::string> result;
        size_t last_pos = 0;
        for (auto pos = input.find(sep); pos != std::string::npos; pos = input.find(sep, last_pos)) {
            result.push_back(input.substr(last_pos, pos - last_pos));
            last_pos = pos + sep.length();
        }
        result.push_back(input.substr(last_pos));
        return result;
    }

    template<class T>
    T convert(const std::string& str) {
        std::stringstream ss(str);
        T result;
        ss >> result;
        return result;
    }
}
