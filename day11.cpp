
#include <unordered_set>
#include <iostream>
#include "registration.h"
#include "point.h"

IMPLEMENT_DAY(11, Day11)

using Grid = std::array<std::array<uint8_t, 10>, 10>;

namespace {
    Grid parse_input(const std::vector<std::string>& lines) {
        Grid result;
        for (size_t y = 0; y < std::min<size_t>(10, lines.size()); ++y) {
            const auto& line = lines[y];
            for (size_t x = 0; x < std::min<size_t>(10, line.size()); ++x) {
                result[y][x] = line[x] - '0';
            }
        }
        return result;
    }

    int take_step(Grid& grid) {
        std::unordered_set<Point> flashing_points;
        std::unordered_set<Point> flashed_points;

        for (size_t y = 0; y < grid.size(); ++y) {
            for (size_t x = 0; x < grid[y].size(); ++x) {
                if (++grid[y][x] == 10) {
                    flashing_points.insert(Point{static_cast<int>(x), static_cast<int>(y)});
                }
            }
        }

        while (!flashing_points.empty()) {
            const auto itr = flashing_points.begin();
            const Point point = *itr;
            flashing_points.erase(itr);
            flashed_points.insert(point);
            grid[point.y][point.x] = 0;

            // flash neighbors
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == -1 && point.x == 0) continue;
                if (dx == 1 && point.x == grid[0].size() - 1) continue;
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    if (dy == -1 && point.y == 0) continue;
                    if (dy == 1 && point.y == grid.size() - 1) continue;
                    const Point neighbor = point + Point{dx, dy};
                    if (flashed_points.find(neighbor) == flashed_points.end() && grid[neighbor.y][neighbor.x] < 10) {
                        if (++grid[neighbor.y][neighbor.x] == 10) {
                            flashing_points.insert(neighbor);
                        }
                    }
                }
            }
        }

        return static_cast<int>(flashed_points.size());
    }

    bool is_zero(const Grid& grid) {
        for (const auto& row : grid) {
            for (const uint8_t cell : row) {
                if (cell != 0) {
                    return false;
                }
            }
        }
        return true;
    }
}

void Day11::part1(const std::vector<std::string> &lines) const {
    Grid grid = parse_input(lines);
    int flash_count = 0;
    for (int i = 0; i < 100; ++i) {
        flash_count += take_step(grid);
    }
    std::cout << flash_count << std::endl;
}

void Day11::part2(const std::vector<std::string> &lines) const {
    Grid grid = parse_input(lines);
    int steps = 0;
    while (!is_zero(grid)) {
        ++steps;
        take_step(grid);
    }
    std::cout << steps << std::endl;
}
