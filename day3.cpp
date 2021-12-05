
#include <iostream>
#include "registration.h"
#include "util.h"

IMPLEMENT_DAY(3, Day3)

namespace {
    std::vector<int> parse_binary(const std::vector<std::string>& lines) {
        std::vector<int> result;
        result.reserve(lines.size());
        for (const auto& line : lines) {
            result.push_back(std::stoi(line, nullptr, 2));
        }
        return result;
    }
}

void Day3::part1(const std::vector<std::string> &lines) const {
    const std::vector<int> nums = parse_binary(lines);
    const int max = *std::max_element(nums.begin(), nums.end());
    int gamma = 0;
    int mask;
    for (mask = 1; mask <= max; mask <<= 1) {
        int one_count = 0;
        for (const int& num : nums) {
            if (num & mask) {
                ++one_count;
            }
        }
        if (one_count * 2 >= nums.size()) {
            gamma |= mask;
        }
    }
    std::cout << (gamma * (~gamma & (mask - 1))) << std::endl;
}

void Day3::part2(const std::vector<std::string> &lines) const {
    std::vector<int> o2_nums = parse_binary(lines);
    std::vector<int> co2_nums = o2_nums;
    int bits = util::bits_required(*std::max_element(o2_nums.begin(), o2_nums.end()));
    for (int bit = bits - 1; bit >= 0; --bit) {
        int mask = 1 << bit;
        int ones_count = 0;
        if (o2_nums.size() > 1) {
            for (const int &num: o2_nums) {
                if (num & mask) {
                    ++ones_count;
                }
            }
            int expected = ones_count * 2 >= o2_nums.size() ? mask : 0;
            o2_nums.erase(
                    std::remove_if(o2_nums.begin(), o2_nums.end(),
                                   [expected, mask](const int &val) { return (val & mask) != expected; }),
                    o2_nums.end());
        }
        if (co2_nums.size() > 1) {
            ones_count = 0;
            for (const int &num: co2_nums) {
                if (num & mask) {
                    ++ones_count;
                }
            }
            int expected = ones_count * 2 >= co2_nums.size() ? 0 : mask;
            co2_nums.erase(
                    std::remove_if(co2_nums.begin(), co2_nums.end(), [expected, mask](const int& val){return (val & mask) != expected;}),
                    co2_nums.end());
        }
    }

    std::cout << (o2_nums[0] * co2_nums[0]) << std::endl;
}
