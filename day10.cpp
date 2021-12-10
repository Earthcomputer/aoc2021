
#include <iostream>
#include "registration.h"

IMPLEMENT_DAY(10, Day10)

namespace {
    bool parse(const std::string& line, std::vector<char>& stack, char& last_char) {
        for (const char c : line) {
            last_char = c;
            switch (c) {
                case '(': case '[': case '{': case '<':
                    stack.push_back(c);
                    break;
                case ')':
                    if (stack.empty() || stack.back() != '(') return true;
                    stack.pop_back();
                    break;
                case ']':
                    if (stack.empty() || stack.back() != '[') return true;
                    stack.pop_back();
                    break;
                case '}':
                    if (stack.empty() || stack.back() != '{') return true;
                    stack.pop_back();
                    break;
                case '>':
                    if (stack.empty() || stack.back() != '<') return true;
                    stack.pop_back();
                    break;
                default: ;
            }
        }
        return false;
    }
}

void Day10::part1(const std::vector<std::string> &lines) const {
    uint64_t result = 0;
    for (const auto& line : lines) {
        std::vector<char> stack;
        char last_char;
        const bool corrupted = parse(line, stack, last_char);
        if (corrupted && !stack.empty()) {
            switch (last_char) {
                case ')': result += 3; break;
                case ']': result += 57; break;
                case '}': result += 1197; break;
                case '>': result += 25137; break;
                default: ;
            }
        }
    }
    std::cout << result << std::endl;
}

void Day10::part2(const std::vector<std::string> &lines) const {
    std::vector<uint64_t> scores;
    for (const auto& line : lines) {
        std::vector<char> stack;
        char last_char;
        const bool corrupted = parse(line, stack, last_char);
        if (!corrupted) {
            uint64_t score = 0;
            for (auto it = stack.rbegin(); it != stack.rend(); ++it) {
                score *= 5;
                switch (*it) {
                    case '(': score += 1; break;
                    case '[': score += 2; break;
                    case '{': score += 3; break;
                    case '<': score += 4; break;
                    default: ;
                }
            }
            scores.push_back(score);
        }
    }
    std::sort(scores.begin(), scores.end());
    std::cout << scores[scores.size() / 2] << std::endl;
}
