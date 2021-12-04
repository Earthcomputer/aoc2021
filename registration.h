#pragma once

#include <map>
#include <utility>
#include <vector>

class Day {
public:
    static std::map<int, Day*>& all_days() noexcept;

    std::string name;

    explicit Day(int day, std::string name) noexcept : name(std::move(name)) {
        all_days()[day] = this;
    }

    virtual void part1(const std::vector<std::string>& lines) const = 0;
    virtual void part2(const std::vector<std::string>& lines) const = 0;
};

#define IMPLEMENT_DAY(number, class_name) class class_name : public Day { \
public:\
    class_name() noexcept : Day(number, #class_name) {}                           \
    virtual void part1(const std::vector<std::string>& lines) const override; \
    virtual void part2(const std::vector<std::string>& lines) const override;\
};                                                                        \
class_name class_name##INSTANCE;
