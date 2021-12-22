
#include <bitset>
#include <iostream>
#include "registration.h"
#include "point3d.h"
#include "util.h"

IMPLEMENT_DAY(22, Day22)

namespace {
    struct Step {
        bool on = false;
        Point3d min;
        Point3d max;
    };

    std::vector<Step> parse_input(const std::vector<std::string>& lines) {
        std::vector<Step> result;
        result.reserve(lines.size());
        for (const auto& line : lines) {
            const auto tokens = util::split(line, " ");
            const auto coords = util::split(tokens[1], ",");
            const auto x_coords = util::split(coords[0].substr(2), "..");
            const auto y_coords = util::split(coords[1].substr(2), "..");
            const auto z_coords = util::split(coords[2].substr(2), "..");
            result.emplace_back(Step{
                tokens[0] == "on",
                Point3d(util::convert<int>(x_coords[0]), util::convert<int>(y_coords[0]), util::convert<int>(z_coords[0])),
                Point3d(util::convert<int>(x_coords[1]), util::convert<int>(y_coords[1]), util::convert<int>(z_coords[1]))});
        }
        return result;
    }

    std::vector<int> make_set(const std::vector<Step>& steps, const std::function<int(const Point3d&)>& projector) {
        std::unordered_set<int> already_inserted;
        std::vector<int> set;
        for (const Step& step : steps) {
            int value = projector(step.min);
            if (already_inserted.insert(value).second) {
                set.push_back(value);
            }
            if (already_inserted.insert(value - 1).second) {
                set.push_back(value - 1);
            }
            if (already_inserted.insert(value + 1).second) {
                set.push_back(value + 1);
            }
            value = projector(step.max);
            if (already_inserted.insert(value).second) {
                set.push_back(value);
            }
            if (already_inserted.insert(value - 1).second) {
                set.push_back(value - 1);
            }
            if (already_inserted.insert(value + 1).second) {
                set.push_back(value + 1);
            }
        }
        std::sort(set.begin(), set.end());
        return set;
    }

    size_t index_of(const std::vector<int>& map, int value) {
        return std::lower_bound(map.begin(), map.end(), value) - map.begin();
    }

    void solve(const std::vector<std::string>& lines, bool part2) {
        const auto steps = parse_input(lines);
        const std::vector<int> x_set = make_set(steps, [](const Point3d& point){return point.x;});
        const std::vector<int> y_set = make_set(steps, [](const Point3d& point){return point.y;});
        const std::vector<int> z_set = make_set(steps, [](const Point3d& point){return point.z;});

        std::vector<std::vector<std::vector<bool>>> map;
        map.reserve(x_set.size());
        for (size_t x = 0; x < x_set.size(); ++x) {
            auto& a = map.emplace_back();
            a.reserve(y_set.size());
            for (size_t y = 0; y < y_set.size(); ++y) {
                a.emplace_back(z_set.size());
            }
        }
        for (const Step& step : steps) {
            if (!part2 && (step.min.x < -50 || step.min.y < -50 || step.min.z < -50 || step.max.x > 50 || step.max.y > 50 || step.max.z > 50)) {
                continue;
            }
            const size_t min_x = index_of(x_set, step.min.x);
            const size_t max_x = index_of(x_set, step.max.x);
            const size_t min_y = index_of(y_set, step.min.y);
            const size_t max_y = index_of(y_set, step.max.y);
            const size_t min_z = index_of(z_set, step.min.z);
            const size_t max_z = index_of(z_set, step.max.z);
            for (size_t x = min_x; x <= max_x; ++x) {
                for (size_t y = min_y; y <= max_y; ++y) {
                    for (size_t z = min_z; z <= max_z; ++z) {
                        map[x][y][z] = step.on;
                    }
                }
            }
        }
        uint64_t result = 0;
        for (int x = 0; x < x_set.size() - 1; ++x) {
            for (int y = 0; y < y_set.size() - 1; ++y) {
                for (int z = 0; z < z_set.size() - 1; ++z) {
                    if (map[x][y][z]) {
                        result += static_cast<uint64_t>(x_set[x + 1] - x_set[x]) * static_cast<uint64_t>(y_set[y + 1] - y_set[y]) * static_cast<uint64_t>(z_set[z + 1] - z_set[z]);
                    }
                }
            }
        }
        std::cout << result << std::endl;
    }
}

void Day22::part1(const std::vector<std::string> &lines) const {
    solve(lines, false);
}

void Day22::part2(const std::vector<std::string> &lines) const {
    solve(lines, true);
}
