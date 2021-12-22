
#include <unordered_set>
#include <iostream>

#include "point3d.h"
#include "registration.h"
#include "util.h"

IMPLEMENT_DAY(19, Day19)

namespace {
    struct Rotation {
        int8_t m00, m10, m20, m01, m11, m21, m02, m12, m22;

        [[nodiscard]] constexpr Point3d rotate(const Point3d& point) const {
            return {
                    m00 * point.x + m01 * point.y + m02 * point.z,
                    m10 * point.x + m11 * point.y + m12 * point.z,
                    m20 * point.x + m21 * point.y + m22 * point.z
                    };
        }
    };

    constexpr Rotation ROTATIONS[24] = {
            {1, 0, 0, 0, 1, 0, 0, 0, 1},
            {1, 0, 0, 0, -1, 0, 0, 0, -1},
            {1, 0, 0, 0, 0, 1, 0, -1, 0},
            {1, 0, 0, 0, 0, -1, 0, 1, 0},
            {-1, 0, 0, 0, 1, 0, 0, 0, -1},
            {-1, 0, 0, 0, -1, 0, 0, 0, 1},
            {-1, 0, 0, 0, 0, 1, 0, 1, 0},
            {-1, 0, 0, 0, 0, -1, 0, -1, 0},
            {0, 1, 0, 1, 0, 0, 0, 0, -1},
            {0, 1, 0, -1, 0, 0, 0, 0, 1},
            {0, 1, 0, 0, 0, 1, 1, 0, 0},
            {0, 1, 0, 0, 0, -1, -1, 0, 0},
            {0, -1, 0, 1, 0, 0, 0, 0, 1},
            {0, -1, 0, -1, 0, 0, 0, 0, -1},
            {0, -1, 0, 0, 0, 1, -1, 0, 0},
            {0, -1, 0, 0, 0, -1, 1, 0, 0},
            {0, 0, 1, 1, 0, 0, 0, 1, 0},
            {0, 0, 1, -1, 0, 0, 0, -1, 0},
            {0, 0, 1, 0, 1, 0, -1, 0, 0},
            {0, 0, 1, 0, -1, 0, 1, 0, 0},
            {0, 0, -1, 1, 0, 0, 0, -1, 0},
            {0, 0, -1, -1, 0, 0, 0, 1, 0},
            {0, 0, -1, 0, 1, 0, 1, 0, 0},
            {0, 0, -1, 0, -1, 0, -1, 0, 0}
    };
}

namespace {
    std::vector<std::vector<Point3d>> parse_input(const std::vector<std::string>& lines) {
        std::vector<std::vector<Point3d>> result;
        std::vector<Point3d>* current_scanner = &result.emplace_back();
        for (const auto& line : lines) {
            if (line.empty()) {
                current_scanner = &result.emplace_back();
            } else if (!line.starts_with("---")) {
                const auto parts = util::split(line, ",");
                current_scanner->emplace_back(util::convert<int>(parts[0]), util::convert<int>(parts[1]), util::convert<int>(parts[2]));
            }
        }
        return result;
    }

    bool find_intersection(
            const std::vector<Point3d>& points_a,
            const std::vector<Point3d>& points_b,
            Point3d& out_origin_a,
            Point3d& out_origin_b,
            const Rotation*& out_rotation
    ) {
        for (const Point3d& origin_a : points_a) {
            std::vector<Point3d> relpoints_a;
            relpoints_a.reserve(points_a.size());
            for (const Point3d& other : points_a) {
                relpoints_a.push_back(other - origin_a);
            }

            for (const Rotation& rotation : ROTATIONS) {
                for (const Point3d& origin_b : points_b) {
                    std::unordered_set<Point3d> relpoints_b;
                    for (const Point3d& other : points_b) {
                        relpoints_b.insert(rotation.rotate(other - origin_b));
                    }
                    int common_count = 0;
                    for (const Point3d& point_a : relpoints_a) {
                        if (relpoints_b.find(point_a) != relpoints_b.end()) {
                            ++common_count;
                        }
                    }
                    if (common_count >= 12) {
                        out_origin_a = origin_a;
                        out_origin_b = origin_b;
                        out_rotation = &rotation;
                        return true;
                    }
                }
            }
        }

        return false;
    }

    void solve(const std::vector<std::string>& lines, bool part2) {
        auto scanners = parse_input(lines);
        std::vector<bool> computed_positions(scanners.size());
        std::vector<Point3d> scanner_positions(scanners.size());
        computed_positions[0] = true;
        size_t num_computed = 1;
        std::vector<size_t> to_visit = {0};
        while (num_computed < scanners.size()) {
            const size_t scanner = to_visit.back();
            to_visit.pop_back();

            for (size_t other_scanner = 0; other_scanner < scanners.size(); ++other_scanner) {
                if (computed_positions[other_scanner]) {
                    continue;
                }
                //std::cout << "Comparing " << scanner << " with " << other_scanner << std::endl;
                Point3d origin_a;
                Point3d origin_b;
                const Rotation* rotation;
                if (find_intersection(scanners[scanner], scanners[other_scanner], origin_a, origin_b, rotation)) {
                    for (Point3d& point : scanners[other_scanner]) {
                        point = rotation->rotate(point - origin_b) + origin_a;
                    }
                    ++num_computed;
                    computed_positions[other_scanner] = true;
                    scanner_positions[other_scanner] = rotation->rotate(Point3d(0, 0, 0) - origin_b) + origin_a;
                    to_visit.push_back(other_scanner);
                }
            }
        }

        if (part2) {
            int max_distance = 0;
            for (size_t a = 0; a < scanners.size() - 1; ++a) {
                const Point3d& pos_a = scanner_positions[a];
                for (size_t b = a + 1; b < scanners.size(); ++b) {
                    const Point3d& pos_b = scanner_positions[b];
                    max_distance = std::max(max_distance, std::abs(pos_b.x - pos_a.x) + std::abs(pos_b.y - pos_a.y) +std::abs(pos_b.z - pos_a.z));
                }
            }
            std::cout << max_distance << std::endl;
        } else {
            std::unordered_set<Point3d> points;
            for (const auto &scanner: scanners) {
                for (const Point3d &point: scanner) {
                    points.insert(point);
                }
            }
            std::cout << points.size() << std::endl;
        }
    }
}

void Day19::part1(const std::vector<std::string> &lines) const {
    solve(lines, false);
}

void Day19::part2(const std::vector<std::string> &lines) const {
    solve(lines, true);
}
