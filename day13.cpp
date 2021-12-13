
#include <unordered_set>
#include <iostream>
#include "registration.h"
#include "point.h"
#include "util.h"

IMPLEMENT_DAY(13, Day13)

namespace {
    struct Fold {
        int pos;
        bool y_axis;
    };

    void parse_input(const std::vector<std::string>& lines, std::unordered_set<Point>& points, std::vector<Fold>& folds) {
        bool in_folds = false;
        for (const auto& line : lines) {
            if (line.empty()) {
                in_folds = true;
                continue;
            }

            if (in_folds) {
                if (line.size() > 11) {
                    const auto parts = util::split(line.substr(11), "=");
                    folds.push_back(Fold{util::convert<int>(parts[1]), parts[0] == "y"});
                }
            } else {
                const auto parts = util::split(line, ",");
                points.insert(Point{util::convert<int>(parts[0]), util::convert<int>(parts[1])});
            }
        }
    }

    void fold(std::unordered_set<Point>& points, const Fold& fold) {
        std::unordered_set<Point> new_points;
        for (const Point& point : points) {
            Point new_point = point;
            if (fold.y_axis) {
                if (point.y > fold.pos) {
                    new_point.y = fold.pos - (point.y - fold.pos);
                }
            } else {
                if (point.x > fold.pos) {
                    new_point.x = fold.pos - (point.x - fold.pos);
                }
            }
            new_points.insert(new_point);
        }
        points = new_points;
    }
}

void Day13::part1(const std::vector<std::string> &lines) const {
    std::unordered_set<Point> points;
    std::vector<Fold> folds;
    parse_input(lines, points, folds);
    fold(points, folds[0]);
    std::cout << points.size() << std::endl;
}

void Day13::part2(const std::vector<std::string> &lines) const {
    std::unordered_set<Point> points;
    std::vector<Fold> folds;
    parse_input(lines, points, folds);
    for (const Fold& f : folds) {
        fold(points, f);
    }
    int min_x = std::min_element(points.begin(), points.end(), [](const Point& a, const Point& b){return a.x < b.x;})->x;
    int min_y = std::min_element(points.begin(), points.end(), [](const Point& a, const Point& b){return a.y < b.y;})->y;
    int max_x = std::max_element(points.begin(), points.end(), [](const Point& a, const Point& b){return a.x < b.x;})->x;
    int max_y = std::max_element(points.begin(), points.end(), [](const Point& a, const Point& b){return a.y < b.y;})->y;
    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            if (points.find(Point{x, y}) == points.end()) {
                std::cout << ".";
            } else {
                std::cout << "#";
            }
        }
        std::cout << std::endl;
    }
}
