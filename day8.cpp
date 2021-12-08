
#include <iostream>
#include "registration.h"
#include "util.h"

IMPLEMENT_DAY(8, Day8)

using Digit = std::array<bool, 7>;

namespace {
    constexpr Digit parse_digit(const std::string_view& input) {
        Digit result{};
        for (bool& val : result) {
            val = false;
        }
        for (const char c : input) {
            const unsigned char index = c - 'a';
            if (index < result.size()) {
                result[index] = true;
            }
        }
        return result;
    }

    constexpr uint8_t true_count(const Digit& digit) {
        int true_count = 0;
        for (const bool val : digit) {
            if (val) {
                ++true_count;
            }
        }
        return true_count;
    }

    constexpr Digit apply_permutation(const Digit& digit, const std::array<uint8_t, 7>& permutation) {
        Digit result{};
        for (int i = 0; i < permutation.size(); ++i) {
            result[permutation[i]] = digit[i];
        }
        return result;
    }

    constexpr std::array<Digit, 10> TABLE({
            parse_digit("abcefg"),
            parse_digit("cf"),
            parse_digit("acdeg"),
            parse_digit("acdfg"),
            parse_digit("bcdf"),
            parse_digit("abdfg"),
            parse_digit("abdefg"),
            parse_digit("acf"),
            parse_digit("abcdefg"),
            parse_digit("abcdfg")
    });

    constexpr std::array<uint8_t, 10> TRUE_COUNTS = [](){
        std::array<uint8_t, 10> result{};
        for (int i = 0; i < result.size(); ++i) {
            result[i] = true_count(TABLE[i]);
        }
        return result;
    }();
}

void Day8::part1(const std::vector<std::string> &lines) const {
    std::vector<Digit> digits;
    digits.reserve(lines.size() * 4);
    for (const auto& line : lines) {
        const auto parts = util::split(line, " | ");
        for (const auto& digit_str : util::split(parts[1], " ")) {
            digits.push_back(parse_digit(digit_str));
        }
    }

    constexpr uint8_t single_counts[] = {
            TRUE_COUNTS[1],
            TRUE_COUNTS[4],
            TRUE_COUNTS[7],
            TRUE_COUNTS[8]
    };
    int count = 0;
    for (const Digit& digit : digits) {
        int tcount = true_count(digit);
        for (uint8_t wanted_count : single_counts) {
            if (tcount == wanted_count) {
                ++count;
                break;
            }
        }
    }
    std::cout << count << std::endl;
}

void Day8::part2(const std::vector<std::string> &lines) const {
    uint32_t total = 0;

    for (const auto& line : lines) {
        const auto parts = util::split(line, " | ");
        std::vector<Digit> all_digits;
        for (const auto& digit_str : util::split(parts[0], " ")) {
            all_digits.push_back(parse_digit(digit_str));
        }
        for (const auto& digit_str : util::split(parts[1], " ")) {
            all_digits.push_back(parse_digit(digit_str));
        }

        std::array<uint8_t, 7> segment_mappings{};
        for (int i = 0; i < segment_mappings.size(); ++i) {
            segment_mappings[i] = i;
        }
        do {
            bool valid = true;
            for (const Digit& digit : all_digits) {
                const Digit permuted_digit = apply_permutation(digit, segment_mappings);
                if (std::find(TABLE.begin(), TABLE.end(), permuted_digit) == TABLE.end()) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                uint32_t result = 0;
                for (const auto& digit_str : util::split(parts[1], " ")) {
                    const Digit scrambled = parse_digit(digit_str);
                    const Digit unscrambled = apply_permutation(scrambled, segment_mappings);
                    const size_t digit = std::find(TABLE.begin(), TABLE.end(), unscrambled) - TABLE.begin();
                    result = result * 10 + static_cast<uint32_t>(digit);
                }
                total += result;
                break;
            }
        } while (std::next_permutation(segment_mappings.begin(), segment_mappings.end()));
    }

    std::cout << total << std::endl;
}
