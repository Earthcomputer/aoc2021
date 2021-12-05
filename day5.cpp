
#include <unordered_set>
#include <iostream>
#include "registration.h"
#include "point.h"
#include "util.h"

IMPLEMENT_DAY(5, Day5)

using Line = std::pair<Point, Point>;

namespace {
    void add_intersections(const Line& a, const Line& b, std::unordered_set<Point>& intersections) {
        if (a.first.x != a.second.x && a.first.y != a.second.y) {
            if (b.first.x != b.second.x && b.first.y != b.second.y) {
                const bool a_positive = (a.first.x < a.second.x) == (a.first.y < a.second.y);
                const bool b_positive = (b.first.x < b.second.x) == (b.first.y < b.second.y);
                if (a_positive == b_positive) {
                    if (a_positive) {
                        if (a.first.y - a.first.x == b.first.y - b.first.x) {
                            int x1 = a.first.x;
                            int x2 = a.second.x;
                            util::minmax(x1, x2);
                            int x3 = b.first.x;
                            int x4 = b.second.x;
                            util::minmax(x3, x4);
                            int y1 = a.first.y;
                            int y2 = a.second.y;
                            util::minmax(y1, y2);
                            int y3 = b.first.y;
                            int y4 = b.second.y;
                            util::minmax(y3, y4);
                            int x_first = std::max(x1, x3);
                            int x_second = std::min(x2, x4);
                            int y_first = std::max(y1, y3);
                            for (int i = 0; i <= x_second - x_first; ++i) {
                                intersections.insert(Point{x_first + i, y_first + i});
                            }
                        }
                    } else {
                        if (a.first.y + a.first.x == b.first.y + b.first.x) {
                            int x1 = a.first.x;
                            int x2 = a.second.x;
                            util::minmax(x1, x2);
                            int x3 = b.first.x;
                            int x4 = b.second.x;
                            util::minmax(x3, x4);
                            int y1 = a.first.y;
                            int y2 = a.second.y;
                            util::minmax(y2, y1);
                            int y3 = b.first.y;
                            int y4 = b.second.y;
                            util::minmax(y4, y3);
                            int x_first = std::max(x1, x3);
                            int x_second = std::min(x2, x4);
                            int y_first = std::min(y1, y3);
                            for (int i = 0; i <= x_second - x_first; ++i) {
                                intersections.insert(Point{x_first + i, y_first - i});
                            }
                        }
                    }
                } else if (b_positive) {
                    add_intersections(b, a, intersections);
                    return;
                } else {
                    if (((a.first.x & 1) == (a.first.y & 1)) == ((b.first.x & 1) == (b.first.y & 1))) {
                        int intersect_x = (b.first.y + b.first.x - a.first.y + a.first.x) >> 1;
                        int x1 = a.first.x;
                        int x2 = a.second.x;
                        util::minmax(x1, x2);
                        int x3 = b.first.x;
                        int x4 = b.second.x;
                        util::minmax(x3, x4);
                        if (intersect_x >= std::max(x1, x3) && intersect_x <= std::min(x2, x4)) {
                            intersections.insert(Point{intersect_x, b.first.y + b.first.x - intersect_x});
                        }
                    }
                }
            } else if (b.first.x == b.second.x) {
                int x1 = a.first.x;
                int x2 = a.second.x;
                util::minmax(x1, x2);
                if (x1 <= b.first.x && b.first.x <= x2) {
                    int y_intersect = (a.first.x < a.second.x) == (a.first.y < a.second.y) ? a.first.y - a.first.x + b.first.x : a.first.y + a.first.x - b.first.x;
                    int y1 = b.first.y;
                    int y2 = b.second.y;
                    util::minmax(y1, y2);
                    if (y1 <= y_intersect && y_intersect <= y2) {
                        intersections.insert(Point{b.first.x, y_intersect});
                    }
                }
            } else {
                int y1 = a.first.y;
                int y2 = a.second.y;
                util::minmax(y1, y2);
                if (y1 <= b.first.y && b.first.y <= y2) {
                    int x_intersect = (a.first.x < a.second.x) == (a.first.y < a.second.y) ? a.first.x - a.first.y + b.first.y : a.first.x + a.first.y - b.first.y;
                    int x1 = b.first.x;
                    int x2 = b.second.x;
                    util::minmax(x1, x2);
                    if (x1 <= x_intersect && x_intersect <= x2) {
                        intersections.insert(Point{x_intersect, b.first.y});
                    }
                }
            }
            return;
        }
        if (b.first.x != b.second.x && b.first.y != b.second.y) {
            add_intersections(b, a, intersections);
            return;
        }
        if ((a.first.x == a.second.x) == (b.first.x == b.second.x)) {
            if (a.first.x == a.second.x && a.first.x == b.first.x) {
                int y1 = a.first.y;
                int y2 = a.second.y;
                util::minmax(y1, y2);
                int y3 = b.first.y;
                int y4 = b.second.y;
                util::minmax(y3, y4);
                for (int y = std::max(y1, y3), e = std::min(y2, y4); y <= e; ++y) {
                    intersections.insert(Point{a.first.x, y});
                }
            } else if (a.first.y == a.second.y && a.first.y == b.first.y) {
                int x1 = a.first.x;
                int x2 = a.second.x;
                util::minmax(x1, x2);
                int x3 = b.first.x;
                int x4 = b.second.x;
                util::minmax(x3, x4);
                for (int x = std::max(x1, x3), e = std::min(x2, x4); x <= e; ++x) {
                    intersections.insert(Point{x, a.first.y});
                }
            }
            return;
        }
        if (a.first.x == a.second.x && b.first.x != b.second.x) {
            add_intersections(b, a, intersections);
            return;
        }
        int x1 = a.first.x;
        int x2 = a.second.x;
        util::minmax(x1, x2);
        int y1 = b.first.y;
        int y2 = b.second.y;
        util::minmax(y1, y2);
        if (x1 <= b.first.x && b.first.x <= x2 && y1 <= a.first.y && a.first.y <= y2) {
            intersections.insert(Point{b.first.x, a.first.y});
        }
    }

    void solve(const std::vector<std::string>& input, bool part2) {
        std::vector<Line> lines;
        for (const auto& l : input) {
            const auto parts = util::split(l, " -> ");
            const auto parts1 = util::split(parts[0], ",");
            const auto parts2 = util::split(parts[1], ",");
            lines.emplace_back(Point{util::convert<int>(parts1[0]), util::convert<int>(parts1[1])}, Point{util::convert<int>(parts2[0]), util::convert<int>(parts2[1])});
            if (!part2 && lines.back().first.x != lines.back().second.x && lines.back().first.y != lines.back().second.y) {
                lines.pop_back();
            }
        }

        std::unordered_set<Point> intersections;
        for (size_t i = 0; i < lines.size() - 1; ++i) {
            for (size_t j = i + 1; j < lines.size(); ++j) {
                add_intersections(lines[i], lines[j], intersections);
            }
        }

        std::cout << intersections.size() << std::endl;
    }
}

void Day5::part1(const std::vector<std::string>& input) const {
    solve(input, false);
}

void Day5::part2(const std::vector<std::string>& input) const {
    solve(input, true);
}
