
#include <iostream>
#include "registration.h"
#include "util.h"

IMPLEMENT_DAY(14, Day14)

namespace {
    void solve(const std::vector<std::string>& lines, int num_iters) {
        const std::string& initial_string = lines[0];
        std::unordered_map<std::pair<char, char>, char> rules;
        for (size_t i = 2; i < lines.size(); ++i) {
            const auto parts = util::split(lines[i], " -> ");
            rules[std::make_pair(parts[0][0], parts[0][1])] = parts[1][0];
        }

        std::unordered_map<char, uint64_t> counts;
        for (const char c : initial_string) {
            counts[c] = counts.emplace(c, 0).first->second + 1;
        }
        std::unordered_map<std::pair<char, char>, uint64_t> pair_counts;
        for (size_t i = 1; i < initial_string.size(); ++i) {
            const auto k = std::make_pair(initial_string[i - 1], initial_string[i]);
            pair_counts[k] = pair_counts.emplace(k, 0).first->second + 1;
        }

        for (int i = 0; i < num_iters; ++i) {
            std::unordered_map<std::pair<char, char>, uint64_t> next_pair_counts;
            for (const auto& pair_count : pair_counts) {
                const auto rule = rules.find(pair_count.first);
                if (rule != rules.end()) {
                    auto k = std::make_pair(pair_count.first.first, rule->second);
                    next_pair_counts[k] = next_pair_counts.emplace(k, 0).first->second + pair_count.second;
                    k = std::make_pair(rule->second, pair_count.first.second);
                    next_pair_counts[k] = next_pair_counts.emplace(k, 0).first->second + pair_count.second;
                    counts[rule->second] = counts.emplace(rule->second, 0).first->second + pair_count.second;
                } else {
                    next_pair_counts[pair_count.first] = next_pair_counts.emplace(pair_count.first, 0).first->second + pair_count.second;
                }
            }
            pair_counts = next_pair_counts;
        }

        uint64_t min_count = std::numeric_limits<uint64_t>::max();
        uint64_t max_count = 0;
        for (const auto& count : counts) {
            if (count.second < min_count) {
                min_count = count.second;
            }
            if (count.second > max_count) {
                max_count = count.second;
            }
        }
        std::cout << (max_count - min_count) << std::endl;
    }
}

void Day14::part1(const std::vector<std::string> &lines) const {
    solve(lines, 10);
}

void Day14::part2(const std::vector<std::string> &lines) const {
    solve(lines, 40);
}
