
#include <iostream>
#include <unordered_set>
#include "registration.h"
#include "point.h"

IMPLEMENT_DAY(9, Day9)

namespace {
    std::vector<std::vector<uint8_t>> parse_input(const std::vector<std::string>& lines) {
        std::vector<std::vector<uint8_t>> map;
        map.reserve(lines.size());
        for (const auto& line : lines) {
            auto& line_vec = map.emplace_back();
            line_vec.reserve(line.size());
            for (const char c : line) {
                line_vec.push_back(c - '0');
            }
        }
        return map;
    }

    std::vector<Point> find_low_points(const std::vector<std::vector<uint8_t>>& map) {
        std::vector<Point> result;
        for (int x = 0; x < map[0].size(); ++x) {
            for (int y = 0; y < map.size(); ++y) {
                uint8_t val = map[y][x];
                uint8_t up = y == 0 ? 10 : map[y-1][x];
                uint8_t down = y == map.size() - 1 ? 10 : map[y+1][x];
                uint8_t left = x == 0 ? 10 : map[y][x-1];
                uint8_t right = x == map[0].size() - 1 ? 10 : map[y][x+1];
                if (val < up && val < down && val < left && val < right) {
                    Point& point = result.emplace_back();
                    point.x = x;
                    point.y = y;
                }
            }
        }
        return result;
    }
}

void Day9::part1(const std::vector<std::string> &lines) const {
    const auto map = parse_input(lines);

    int result = 0;
    for (const Point& low_point : find_low_points(map)) {
        result += 1 + map[low_point.y][low_point.x];
    }
    std::cout << result << std::endl;
}

void Day9::part2(const std::vector<std::string> &lines) const {
    const auto map = parse_input(lines);
    std::vector<size_t> basin_sizes;

    constexpr Point DIRS[] = {
            {0, -1},
            {0, 1},
            {-1, 0},
            {1, 0}
    };

    for (const Point& low_point : find_low_points(map)) {
        std::unordered_set<Point> visited;
        std::unordered_set<Point> to_visit;
        to_visit.insert(low_point);
        while (!to_visit.empty()) {
            const auto& itr = to_visit.begin();
            const Point point = *itr;
            to_visit.erase(itr);
            visited.insert(point);

            for (const Point& dir : DIRS) {
                const Point other_point = point + dir;
                if (other_point.x >= 0 && other_point.y >= 0 && other_point.x < map[0].size() && other_point.y < map.size()
                        && map[other_point.y][other_point.x] != 9 && visited.find(other_point) == visited.end()) {
                    to_visit.insert(other_point);
                }
            }
        }

        basin_sizes.push_back(visited.size());
    }

    std::sort(basin_sizes.begin(), basin_sizes.end());
    std::cout << (basin_sizes[basin_sizes.size() - 1] * basin_sizes[basin_sizes.size() - 2] * basin_sizes[basin_sizes.size() - 3]) << std::endl;
}
