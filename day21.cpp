
#include <iostream>
#include "registration.h"
#include "util.h"

IMPLEMENT_DAY(21, Day21)

namespace {
    std::pair<uint32_t, uint32_t> parse_input(const std::vector<std::string>& lines) {
        constexpr size_t prefix_len = std::string_view("Player 1 starting position: ").size();
        return std::make_pair(util::convert<uint32_t>(lines[0].substr(prefix_len)), util::convert<uint32_t>(lines[1].substr(prefix_len)));
    }
}

void Day21::part1(const std::vector<std::string> &lines) const {
    std::pair<uint32_t, uint32_t> player_positions = parse_input(lines);
    bool player2 = false;
    uint32_t score1 = 0;
    uint32_t score2 = 0;
    uint32_t d;
    for (d = 6; score1 < 1000 && score2 < 1000; d += 9) {
        if (player2) {
            player_positions.second = (player_positions.second + d - 1) % 10 + 1;
            score2 += player_positions.second;
            player2 = false;
        } else {
            player_positions.first = (player_positions.first + d - 1) % 10 + 1;
            score1 += player_positions.first;
            player2 = true;
        }
    }
    std::cout << (std::min(score1, score2) * (d / 9 * 3)) << std::endl;
}

void Day21::part2(const std::vector<std::string> &lines) const {
    const std::pair<uint32_t, uint32_t> starting_positions = parse_input(lines);

    constexpr auto frequencies = []() {
        std::array<int, 10> three_dice_frequencies{};
        for (int i = 0; i < 27; ++i) {
            ++three_dice_frequencies[i % 3 + i / 3 % 3 + i / 9 + 3];
        }
        std::array<std::array<int, 10>, 10> result{};
        for (int i = 3; i < 10; ++i) {
            for (int j = 3; j < 10; ++j) {
                result[i][j] = three_dice_frequencies[i] * three_dice_frequencies[j];
            }
        }
        return result;
    }();

    std::array<std::array<std::unordered_map<std::pair<uint32_t, uint32_t>, uint64_t>, 22>, 22> table;
    table[0][0][starting_positions] = 1;
    for (int x = 0; x < 21; ++x) {
        for (int y = 0; y < 21; ++y) {
            const auto& counts = table[y][x];
            for (const auto& pair : counts) {
                for (int d1 = 3; d1 < 10; ++d1) {
                    const uint32_t new_pos1 = (pair.first.first + d1 - 1) % 10 + 1;
                    const int new_score1 = std::min(21, x + static_cast<int>(new_pos1));
                    if (new_score1 == 21) {
                        const std::pair<uint32_t, uint32_t> new_pos = std::make_pair(new_pos1, pair.first.second);
                        table[y][21].emplace(new_pos, 0).first->second += pair.second * frequencies[3][d1];
                    } else {
                        for (int d2 = 3; d2 < 10; ++d2) {
                            const uint32_t new_pos2 = (pair.first.second + d2 - 1) % 10 + 1;
                            const int new_score2 = std::min(21, y + static_cast<int>(new_pos2));
                            const std::pair<uint32_t, uint32_t> new_pos = std::make_pair(new_pos1, new_pos2);
                            table[new_score2][new_score1].emplace(new_pos, 0).first->second +=pair.second * frequencies[d2][d1];
                        }
                    }
                }
            }
        }
    }

    uint64_t player1_wins = 0;
    for (int y = 0; y < 21; ++y) {
        for (const auto& counts : table[y][21]) {
            player1_wins += counts.second;
        }
    }
    uint64_t player2_wins = 0;
    for (int x = 0; x < 21; ++x) {
        for (const auto& counts : table[21][x]) {
            player2_wins += counts.second;
        }
    }
    std::cout << std::max(player1_wins, player2_wins) << std::endl;
}
