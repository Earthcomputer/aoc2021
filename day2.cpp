
#include <iostream>
#include "registration.h"
#include "util.h"

IMPLEMENT_DAY(2, Day2)

void Day2::part1(const std::vector<std::string> &lines) const {
    int32_t h_pos = 0;
    int32_t depth = 0;
    for (const auto& line : lines) {
        const auto parts = util::split(line, " ");
        const auto& type = parts[0];
        const auto amount = util::convert<int32_t>(parts[1]);
        if (type == "forward") {
            h_pos += amount;
        } else if (type == "up") {
            depth -= amount;
        } else {
            depth += amount;
        }
    }
    std::cout << (h_pos * depth) << std::endl;
}

void Day2::part2(const std::vector<std::string> &lines) const {
    int32_t h_pos = 0;
    int32_t depth = 0;
    int32_t aim = 0;
    for (const auto& line : lines) {
        const auto parts = util::split(line, " ");
        const auto& type = parts[0];
        const auto amount = util::convert<int32_t>(parts[1]);
        if (type == "forward") {
            h_pos += amount;
            depth += amount * aim;
        } else if (type == "up") {
            aim -= amount;
        } else {
            aim += amount;
        }
    }
    std::cout << (h_pos * depth) << std::endl;
}
