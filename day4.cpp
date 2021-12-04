
#include <set>
#include <iostream>
#include "registration.h"
#include "util.h"

IMPLEMENT_DAY(4, Day4)

using BingoCard = std::vector<std::vector<int>>;

namespace {
    bool has_won(const std::set<int>& called, const BingoCard& bingo_card) {
        for (const auto& row : bingo_card) {
            bool has_won = true;
            for (int val : row) {
                if (!called.contains(val)) {
                    has_won = false;
                    break;
                }
            }
            if (has_won) {
                return true;
            }
        }
        for (int x = 0; x < bingo_card[0].size(); ++x) {
            bool has_won = true;
            for (const auto& row : bingo_card) {
                if (!called.contains(row[x])) {
                    has_won = false;
                    break;
                }
            }
            if (has_won) {
                return true;
            }
        }
        return false;
    }

    void solve(const std::vector<std::string>& lines, bool is_part_2) {
        std::vector<int> bingo_nums;
        for (const auto& str : util::split(lines[0], ",")) {
            bingo_nums.push_back(util::convert<int>(str));
        }

        std::vector<BingoCard> bingo_cards;
        BingoCard current_card;
        for (size_t i = 2; i < lines.size(); ++i) {
            const auto& line = lines[i];
            if (line.empty()) {
                bingo_cards.push_back(current_card);
                current_card = BingoCard();
            } else {
                std::vector<int>& nums = current_card.emplace_back();
                for (const auto& num_str : util::split(line, " ")) {
                    if (!num_str.empty()) {
                        nums.push_back(util::convert<int>(num_str));
                    }
                }
            }
        }
        bingo_cards.push_back(current_card);

        std::set<int> called;
        const BingoCard* result_card = nullptr;
        for (const int call : bingo_nums) {
            called.insert(call);
            for (auto i = bingo_cards.size(); i-- > 0;) {
                const BingoCard& card = bingo_cards[i];
                for (const auto& line : card) {
                    if (has_won(called, card)) {
                        // this card has won
                        result_card = &card;
                        if (is_part_2 && bingo_cards.size() > 1) {
                            bingo_cards.erase(bingo_cards.begin() + static_cast<BingoCard::difference_type>(i));
                            break;
                        } else {
                            goto end;
                        }
                    }
                }
            }
        }
        end:
        if (result_card) {
            int result = 0;
            for (const auto& line : *result_card) {
                for (const int& val : line) {
                    if (!called.contains(val)) {
                        result += val;
                    }
                }
            }
            const int last_called = bingo_nums[called.size() - 1];
            std::cout << (result * last_called) << std::endl;
        }
    }
}

void Day4::part1(const std::vector<std::string> &lines) const {
    solve(lines, false);
}

void Day4::part2(const std::vector<std::string> &lines) const {
    solve(lines, true);
}
