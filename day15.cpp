
#include <iostream>
#include <set>
#include "registration.h"
#include "point.h"

IMPLEMENT_DAY(15, Day15)

using Grid = std::vector<std::vector<int>>;

namespace {
    Grid parse_input(const std::vector<std::string>& lines) {
        Grid grid;
        grid.reserve(lines.size());
        for (const auto& line : lines) {
            auto& row = grid.emplace_back();
            row.reserve(line.size());
            for (const char c : line) {
                row.push_back(c - '0');
            }
        }
        return grid;
    }

    int min_distance(const Grid& grid) {
        constexpr Point DIRS[] = {
                Point{-1, 0},
                Point{1, 0},
                Point{0, -1},
                Point{0, 1}
        };
        Grid distances = grid;
        for (auto& row : distances) {
            std::fill(row.begin(), row.end(), std::numeric_limits<int>::max());
        }
        distances[0][0] = 0;
        const auto cmp = [&distances](const Point& a, const Point& b) {
            if (distances[a.y][a.x] != distances[b.y][b.x]) {
                return distances[a.y][a.x] < distances[b.y][b.x];
            }
            if (a.x != b.x) {
                return a.x < b.x;
            }
            return a.y < b.y;
        };
        std::set<Point, decltype(cmp)> to_visit(cmp);
        to_visit.insert(Point{0, 0});
        while (true) {
            const auto itr = to_visit.begin();
            const Point point = *itr;
            to_visit.erase(itr);
            const int distance = distances[point.y][point.x];
            if (point.y == grid.size() - 1 && point.x == grid.back().size() - 1) {
                return distance;
            }
            for (const Point& dir : DIRS) {
                const Point neighbor = point + dir;
                if (neighbor.x < 0 || neighbor.y < 0 || neighbor.x >= grid[0].size() || neighbor.y >= grid.size()) {
                    continue;
                }
                if (distance + grid[neighbor.y][neighbor.x] < distances[neighbor.y][neighbor.x]) {
                    to_visit.erase(neighbor);
                    distances[neighbor.y][neighbor.x] = distance + grid[neighbor.y][neighbor.x];
                    to_visit.insert(neighbor);
                }
            }
        }
    }
}

void Day15::part1(const std::vector<std::string> &lines) const {
    const Grid grid = parse_input(lines);
    std::cout << min_distance(grid) << std::endl;
}

void Day15::part2(const std::vector<std::string> &lines) const {
    const Grid temp_grid = parse_input(lines);
    Grid grid;
    grid.reserve(temp_grid.size() * 5);
    for (int y = 0; y < 5; ++y) {
        for (size_t i = 0; i < temp_grid.size(); ++i) {
            auto& row = grid.emplace_back();
            row.reserve(temp_grid[0].size() * 5);
        }
        for (int x = 0; x < 5; ++x) {
            for (int gy = 0; gy < temp_grid.size(); ++gy) {
                for (int gx = 0; gx < temp_grid[gy].size(); ++gx) {
                    grid[y * temp_grid.size() + gy].push_back((temp_grid[gy][gx] + y + x - 1) % 9 + 1);
                }
            }
        }
    }

    std::cout << min_distance(grid) << std::endl;
}
