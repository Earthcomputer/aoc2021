
#include <bitset>
#include <iostream>
#include "registration.h"

IMPLEMENT_DAY(20, Day20)

namespace {
    void parse_input(const std::vector<std::string>& lines, std::bitset<512>& algorithm, std::vector<bool>& image, size_t& image_width) {
        for (size_t i = 0; i < 512 && i < lines[0].size(); ++i) {
            algorithm[i] = lines[0][i] == '#';
        }

        image_width = lines[2].size();
        image.reserve(image_width * (lines.size() - 2));
        for (size_t i = 2; i < lines.size(); ++i) {
            for (size_t j = 0; j < image_width; ++j) {
                image.push_back(lines[i][j] == '#');
            }
        }
    }

    void enhance_image(const std::bitset<512>& algorithm, const std::vector<bool>& image, size_t image_width, std::vector<bool>& output_image, const bool outside_val) {
        const size_t image_height = image.size() / image_width;
        const size_t new_width = image_width + 2;
        const size_t new_height = image_height + 2;
        output_image.reserve(new_width * new_height);
        for (size_t new_y = 0; new_y < new_height; ++new_y) {
            for (size_t new_x = 0; new_x < new_width; ++new_x) {
                size_t index = 0;
                for (int dy = -1; dy <= 1; ++dy) {
                    const int y = static_cast<int>(new_y) + dy - 1;
                    for (int dx = -1; dx <= 1; ++dx) {
                        const int x = static_cast<int>(new_x) + dx - 1;
                        const bool val = x >= 0 && y >= 0 && x < image_width && y < image_height ? image[y * image_width + x] : algorithm[0] && outside_val;
                        index = (index << 1) | (val ? 1 : 0);
                    }
                }
                output_image.push_back(algorithm[index]);
            }
        }
    }

    void solve(const std::vector<std::string>& lines, int iters) {
        std::bitset<512> algorithm;
        std::vector<bool> image;
        size_t image_width;
        parse_input(lines, algorithm, image, image_width);
        for (int i = 0; i < iters; ++i) {
            std::vector<bool> new_image;
            enhance_image(algorithm, image, image_width, new_image, i & 1);
            image_width += 2;
            image = new_image;
        }
        int count = 0;
        for (const auto& b : image) {
            if (b) {
                ++count;
            }
        }
        std::cout << count << std::endl;
    }
}

void Day20::part1(const std::vector<std::string> &lines) const {
    solve(lines, 2);
}

void Day20::part2(const std::vector<std::string> &lines) const {
    solve(lines, 50);
}
