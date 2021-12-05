
#include <iostream>
#include "registration.h"
#include "util.h"

IMPLEMENT_DAY(1, Day1)

void Day1::part1(const std::vector<std::string> &lines) const {
    int increased = 0;
    for (size_t i = 1; i < lines.size(); ++i) {
        if (util::convert<int>(lines[i]) > util::convert<int>(lines[i - 1])) {
            ++increased;
        }
    }
    std::cout << increased << std::endl;
}

void Day1::part2(const std::vector<std::string> &lines) const {
    int increased = 0;
    for (size_t i = 3; i < lines.size(); ++i) {
        if (util::convert<int>(lines[i]) > util::convert<int>(lines[i - 3])) {
            ++increased;
        }
    }
    std::cout << increased << std::endl;
}
