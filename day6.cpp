
#include <cstring>
#include <iostream>
#include "registration.h"
#include "util.h"

IMPLEMENT_DAY(6, Day6)

namespace {
    void solve(const std::vector<std::string>& lines, int count) {
        uint64_t counts[9];
        memset(counts, 0, sizeof(counts));
        for (const auto& num : util::split(lines[0], ",")) {
            const auto val = util::convert<size_t>(num);
            if (val < sizeof(counts) / sizeof(*counts)) {
                ++counts[val];
            }
        }
        for (int i = 0; i < count; ++i) {
            uint64_t zero_count = *counts;
            for (int j = 1; j < sizeof(counts) / sizeof(*counts); ++j) {
                counts[j - 1] = counts[j];
            }
            counts[6] += zero_count;
            counts[8] = zero_count;
        }
        uint64_t total = 0;
        for (uint64_t val : counts) {
            total += val;
        }
        std::cout << total << std::endl;
    }
}

void Day6::part1(const std::vector<std::string> &lines) const {
    solve(lines, 80);
}

void Day6::part2(const std::vector<std::string> &lines) const {
    solve(lines, 256);
}
