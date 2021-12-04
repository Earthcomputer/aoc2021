#include <iostream>
#include <fstream>

#include "registration.h"

int main() {
    std::cout << "Enter day:" << std::endl;
    int day;
    std::cin >> day;
    const auto day_itr = Day::all_days().find(day);
    if (day_itr == Day::all_days().end()) {
        std::cout << "Day not found" << std::endl;
    } else {
        const Day* pDay = day_itr->second;
        std::cout << "Enter part:" << std::endl;
        int part;
        std::cin >> part;
        std::ifstream input(pDay->name + ".txt");
        std::string line;
        std::vector<std::string> lines;
        while (std::getline(input, line)) {
            lines.push_back(line);
        }
        while (!lines.empty() && lines[0].empty()) {
            lines.erase(lines.begin());
        }
        while (!lines.empty() && lines.back().empty()) {
            lines.pop_back();
        }
        if (part == 1) {
            pDay->part1(lines);
        } else if (part == 2) {
            pDay->part2(lines);
        } else {
            std::cout << "Invalid part" << std::endl;
        }
    }
    return 0;
}
