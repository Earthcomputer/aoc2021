
#include <iostream>
#include "registration.h"
#include "point.h"
#include "util.h"

IMPLEMENT_DAY(17, Day17)

namespace {
    std::pair<Point, Point> parse_input(const std::vector<std::string>& lines) {
        constexpr size_t prefix_len = std::string_view("target area: x=").size();
        const auto parts = util::split(lines[0].substr(prefix_len), ", y=");
        const auto x_parts = util::split(parts[0], "..");
        const auto y_parts = util::split(parts[1], "..");
        return std::make_pair(
                Point{util::convert<int>(x_parts[0]), util::convert<int>(y_parts[0])},
                Point{util::convert<int>(x_parts[1]), util::convert<int>(y_parts[1])});
    }

    constexpr int triangle(int n) {
        return ((n + 1) * n) / 2;
    }

    int inv_triangle_floor(int triangle) {
        // x = (v^2 + v) / 2
        // 2x = v^2 + v
        // v^2 + v - 2x = 0
        // v = (sqrt(1 + 8x) - 1)/2
        return static_cast<int>(std::floor((std::sqrt(1 + 8 * triangle) - 1) * 0.5));
    }

    int inv_triangle_ceil(int triangle) {
        return static_cast<int>(std::ceil((std::sqrt(1 + 8 * triangle) - 1) * 0.5));
    }

    int num_ways_to_overlap(int steps, int min, int max) {
        if (min > max) {
            return 0;
        }
        if (steps & 1) {
            // for odd numbers, it's the number of multiples of steps there are in the target range
            int min_multiple = min / steps * steps;
            if (min_multiple < min) {
                min_multiple += steps;
            }
            int max_multiple = max / steps * steps;
            return (max_multiple - min_multiple) / steps + 1;
        } else {
            // for even numbers, it's the number of anti-multiples (multiples + steps/2) there are in the target range
            int min_anti_multiple = min / (steps / 2) * (steps / 2);
            if ((min_anti_multiple % steps) == 0) {
                min_anti_multiple += steps / 2;
            }
            if (min_anti_multiple < min) {
                min_anti_multiple += steps;
            }
            int max_anti_multiple = max / (steps / 2) * (steps / 2);
            if ((max_anti_multiple % steps) == 0) {
                max_anti_multiple -= steps / 2;
            }
            return (max_anti_multiple - min_anti_multiple) / steps + 1;
        }
    }
}

void Day17::part1(const std::vector<std::string> &lines) const {
    const auto input = parse_input(lines);
    int min_x = std::abs(input.first.x);
    int max_x = std::abs(input.second.x);
    util::minmax(min_x, max_x);
    int min_y = std::abs(input.first.y);
    int max_y = std::abs(input.second.y);
    util::minmax(min_y, max_y);

    bool exact_steps;
    int steps;
    if ((input.first.x < 0) != (input.second.x < 0)) {
        exact_steps = false;
        steps = 0;
    } else {
        int floor_v = inv_triangle_floor(min_x);
        int min_triangle = triangle(floor_v);
        int next_triangle = triangle(floor_v + 1);
        if (min_triangle >= min_x) {
            exact_steps = false;
            steps = floor_v;
        } else if (next_triangle <= max_x) {
            exact_steps = false;
            steps = floor_v + 1;
            // we actually can hit the box earlier
            int undershoot = next_triangle - min_x;
            steps -= inv_triangle_floor(undershoot);
        } else {
            exact_steps = true;
            int overshoot = next_triangle - max_x;
            // find n for the smallest triangle number >= overshoot
            int n = inv_triangle_ceil(overshoot);
            steps = floor_v + 1 - n;
        }
    }

    int vy;
    if (exact_steps) {
        vy = (steps - 1) / 2;
    } else {
        if (steps <= max_y - 1) {
            vy = max_y - 1;
        } else {
            vy = (steps - 1) / 2;
        }
    }

    int highest_pos = triangle(vy);
    std::cout << highest_pos << std::endl;
}

#define USE_BRUTE_FORCE

#ifdef USE_BRUTE_FORCE

void Day17::part2(const std::vector<std::string> &lines) const {
    const auto input = parse_input(lines);
    int min_x = std::abs(input.first.x);
    int max_x = std::abs(input.second.x);
    util::minmax(min_x, max_x);
    int min_y = std::abs(input.first.y);
    int max_y = std::abs(input.second.y);
    util::minmax(min_y, max_y);

    const auto hits_target = [=](int vx, int vy) {
        int x = 0;
        int y = 0;
        while (y <= max_y) {
            if (min_x <= x && x <= max_x && min_y <= y && y <= max_y) {
                return true;
            }
            x += vx;
            y += vy;
            vx--;
            if (vx < 0) vx = 0;
            vy++;
        }
        return false;
    };

    int result = 0;
    for (int vx = 0; vx < 1000; ++vx) {
        for (int vy = -1000; vy < 1000; ++vy) {
            if (hits_target(vx, vy)) {
                ++result;
            }
        }
    }

    std::cout << result << std::endl;
}

#else

void Day17::part2(const std::vector<std::string> &lines) const {
    const auto input = parse_input(lines);
    int min_x = std::abs(input.first.x);
    int max_x = std::abs(input.second.x);
    util::minmax(min_x, max_x);
    int min_y = std::abs(input.first.y);
    int max_y = std::abs(input.second.y);
    util::minmax(min_y, max_y);

    int min_vertical_steps = inv_triangle_ceil(min_x);
    int max_vertical_steps = inv_triangle_floor(max_x);

    int result = 0;

    int max_steps = max_y * 2 + 1;
    for (int steps = 1; steps < max_steps; ++steps) {
        // find the number of ways it's possible to reach the target area in the x in this number of steps
        int num_ways_x;
        int effective_min_x = std::max(min_x, triangle(steps)); // workaround for vx going backwards
        if (steps <= max_vertical_steps) {
            num_ways_x = num_ways_to_overlap(steps, effective_min_x, max_x);
        } else {
            num_ways_x = 0;
        }
        if (steps > min_vertical_steps) {
            // handle the triangle numbers in the target x's having an unbounded number of steps
            num_ways_x += std::min(steps, max_vertical_steps) - min_vertical_steps;
        }

        // find the number of ways it's possible to reach the target area in the y in this number of steps
        int num_ways_y = num_ways_to_overlap(steps, min_y, max_y);

        result += num_ways_x * num_ways_y;

        if (steps > 1) {
            // some trajectories may have more than one overlap, remove the duplicates
            // xPrev = x - (x / steps) +- (steps - 1) / 2
            // 2*xPrev = 2x - 2x / steps +- steps -+ 1
            // 2x - 2x / steps = 2*xPrev -+ steps +- 1
            // x = (2*xPrev -+ steps +- 1) / (1 - 1/steps)
            // x = (2*xPrev -+ steps +- 1) / ((steps - 1) / steps)
            // x = steps * (2*xPrev -+ steps +- 1) / (steps - 1)
            const auto threshold = [steps](int xPrev, bool accelerating) {
                // + steps - 2 for ceil of division
                return (steps * (2 * xPrev + (steps - 1) * (accelerating ? 1 : -1)) + steps - 2) / (steps - 1);
            };
            int duplicates_x = num_ways_to_overlap(steps, threshold(effective_min_x, false), max_x);
            duplicates_x += std::max(0, std::min(steps, max_vertical_steps + 1) - min_vertical_steps - 1);
            int duplicates_y = num_ways_to_overlap(steps, threshold(min_y, true), max_y);
            result -= duplicates_x * duplicates_y;
        }
    }

    std::cout << result << std::endl;
}

#endif
