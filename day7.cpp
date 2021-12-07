
#include <iostream>
#include <cmath>
#include "registration.h"
#include "util.h"

IMPLEMENT_DAY(7, Day7)

void Day7::part1(const std::vector<std::string> &lines) const {
    std::vector<int> positions;
    for (const auto& str : util::split(lines[0], ",")) {
        positions.push_back(util::convert<int>(str));
    }
    std::sort(positions.begin(), positions.end());
    int median = positions[positions.size() / 2];
    uint64_t sum = 0;
    for (int pos : positions) {
        sum += static_cast<uint64_t>(std::abs(median - pos));
    }
    std::cout << sum << std::endl;
}

namespace {
    uint64_t twice_cost(const std::vector<int>& positions, int value) {
        uint64_t result = 0;
        for (int pos : positions) {
            result += (pos - value) * (pos - value) + std::abs(pos - value);
        }
        return result;
    }
}

void Day7::part2(const std::vector<std::string> &lines) const {
    std::vector<int> positions;
    for (const auto& str : util::split(lines[0], ",")) {
        positions.push_back(util::convert<int>(str));
    }
    // y = (x-a)^2+|x-a| + (x-b)^2+|x-b| + ...
    // dy/dx = 2(x-a)+sign(x-a) + 2(x-b)+sign(x-b) + ... = 2nx - 2sum + sign(x-a)+sign(x-b)+...
    // 0 = 2nx - 2sum + sign(x-a)+sign(x-b)+... <=> x = (2sum - sign(x-a)-sign(x-b)-...)/2n = sum/n - [-1, 1]/2

    int64_t sum = 0;
    for (int pos : positions) {
        sum += pos;
    }
    int mean = static_cast<int>(round(static_cast<double>(sum) / static_cast<double>(positions.size())));
    auto twice_mean_cost = twice_cost(positions, mean);
    auto twice_neg_cost = twice_cost(positions, mean - 1);
    auto twice_pos_cost = twice_cost(positions, mean + 1);
    auto twice_min_cost = std::min({twice_mean_cost, twice_neg_cost, twice_pos_cost});
    std::cout << (twice_min_cost / 2) << std::endl;
}
