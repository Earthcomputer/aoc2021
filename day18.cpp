
#include <variant>
#include <memory>
#include <iostream>
#include "registration.h"
#include "util.h"

IMPLEMENT_DAY(18, Day18)

namespace {
    class ShellfishNumber;
}
using ShellfishNumberPtr = std::shared_ptr<ShellfishNumber>;
using ShellfishPair = std::pair<ShellfishNumberPtr, ShellfishNumberPtr>;

namespace {
    class ShellfishNumber : std::variant<int, ShellfishPair> {
        using Super = std::variant<int, ShellfishPair>;

        [[nodiscard]] ShellfishNumber copy() const {
            if (std::holds_alternative<int>(*this)) {
                return std::get<int>(*this);
            } else {
                const auto& this_pair = std::get<ShellfishPair>(*this);
                return std::make_pair<ShellfishNumberPtr, ShellfishNumberPtr>(std::make_shared<ShellfishNumber>(this_pair.first->copy()), std::make_shared<ShellfishNumber>(this_pair.second->copy()));
            }
        }

    public:
        ShellfishNumber(const ShellfishNumber& other) : Super(std::move(other.copy())) {}
        template<class T>
        ShellfishNumber(T&& t) : Super(t) {}

        ShellfishNumber& operator=(const ShellfishNumber& other) {
            Super::operator=(other);
            if (std::holds_alternative<ShellfishPair>(*this)) {
                auto& this_pair = std::get<ShellfishPair>(*this);
                this_pair.first = std::make_shared<ShellfishNumber>(*this_pair.first);
                this_pair.second = std::make_shared<ShellfishNumber>(*this_pair.second);
            }
            return *this;
        }

        void reduce() {
            if (std::holds_alternative<int>(*this)) {
                return;
            }
            auto& this_pair = std::get<ShellfishPair>(*this);

            const auto get_pair = [](const ShellfishNumberPtr& ptr) {
                return std::holds_alternative<int>(*ptr) ? nullptr : &std::get<ShellfishPair>(*ptr);
            };

            while (true) {
                for (int i = 0; i < 16; ++i) {
                    ShellfishPair* pair = &this_pair;
                    ShellfishPair* parent;
                    for (int m = 8; m != 0; m >>= 1) {
                        parent = pair;
                        pair = pair ? (i & m ? get_pair(pair->second) : get_pair(pair->first)) : nullptr;
                    }
                    if (pair) {
                        // explode
                        const auto explode_to = [&this_pair, get_pair](int i, int val, bool align_left) {
                            ShellfishPair* pair = &this_pair;
                            for (int m = 8; m != 0; m >>= 1) {
                                ShellfishPair* next_pair = i & m ? get_pair(pair->second) : get_pair(pair->first);
                                if (next_pair) {
                                    pair = next_pair;
                                } else {
                                    if (i & m) {
                                        std::get<int>(*pair->second) += val;
                                    } else {
                                        std::get<int>(*pair->first) += val;
                                    }
                                    return;
                                }
                            }
                            while (pair) {
                                ShellfishPair* next_pair = get_pair(align_left ? pair->first : pair->second);
                                if (!next_pair) {
                                    if (align_left) {
                                        std::get<int>(*pair->first) += val;
                                    } else {
                                        std::get<int>(*pair->second) += val;
                                    }
                                }
                                pair = next_pair;
                            }
                        };
                        if (i != 0) {
                            explode_to(i - 1, std::get<int>(*pair->first), false);
                        }
                        if (i != 15) {
                            explode_to(i + 1, std::get<int>(*pair->second), true);
                        }

                        if (i & 1) {
                            *parent->second = 0;
                        } else {
                            *parent->first = 0;
                        }

                        goto did_stuff;
                    }
                }

                for (int i = 0; i < 16; ++i) {
                    ShellfishPair* pair = &this_pair;
                    for (int m = 8; m != 0; m >>= 1) {
                        ShellfishNumber& v = i & m ? *pair->second : *pair->first;
                        if (std::holds_alternative<int>(v)) {
                            const int val = std::get<int>(v);
                            if (val >= 10) {
                                // split
                                v = std::make_pair<ShellfishNumberPtr, ShellfishNumberPtr>(
                                        std::make_shared<ShellfishNumber>(val / 2),
                                        std::make_shared<ShellfishNumber>((val + 1) / 2));
                                goto did_stuff;
                            }
                        } else {
                            pair = &std::get<ShellfishPair>(v);
                        }
                    }
                }

                break;
                did_stuff: ;
            }
        }

        ShellfishNumber operator+(const ShellfishNumber& other) const {
            ShellfishNumber result = std::make_pair<ShellfishNumberPtr, ShellfishNumberPtr>(std::make_shared<ShellfishNumber>(*this), std::make_shared<ShellfishNumber>(other));
            result.reduce();
            return result;
        }

        [[nodiscard]] int magnitude() const {
            if (std::holds_alternative<int>(*this)) {
                return std::get<int>(*this);
            } else {
                const auto& pair = std::get<ShellfishPair>(*this);
                return 3 * pair.first->magnitude() + 2 * pair.second->magnitude();
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const ShellfishNumber& value);
    };

    std::ostream& operator<<(std::ostream& os, const ShellfishNumber& value) {
        if (std::holds_alternative<int>(value)) {
            os << std::get<int>(value);
        } else {
            const auto& pair = std::get<ShellfishPair>(value);
            os << "[" << *pair.first << "," << *pair.second << "]";
        }
        return os;
    }
}
namespace {
    ShellfishNumber parse_shellfish_number(const std::string& str, int& index) {
        if (str[index] == '[') {
            ++index;
            const ShellfishNumber left = parse_shellfish_number(str, index);
            ++index; // ,
            const ShellfishNumber right = parse_shellfish_number(str, index);
            ++index; // ]
            return std::make_pair<ShellfishNumberPtr, ShellfishNumberPtr>(std::make_shared<ShellfishNumber>(left), std::make_shared<ShellfishNumber>(right));
        } else {
            const int start_index = index;
            while (index < str.size() && std::isdigit(str[index])) {
                ++index;
            }
            return util::convert<int>(str.substr(start_index, index));
        }
    }

    ShellfishNumber parse_shellfish_number(const std::string& str) {
        int index = 0;
        return parse_shellfish_number(str, index);
    }
}

void Day18::part1(const std::vector<std::string> &lines) const {
    ShellfishNumber result = parse_shellfish_number(lines[0]);
    for (size_t i = 1; i < lines.size(); ++i) {
        result = result + parse_shellfish_number(lines[i]);
    }
    std::cout << result.magnitude() << std::endl;
}

void Day18::part2(const std::vector<std::string> &lines) const {
    std::vector<ShellfishNumber> numbers;
    numbers.reserve(lines.size());
    for (const auto& line : lines) {
        numbers.push_back(parse_shellfish_number(line));
    }
    int max_magnitude = 0;
    for (size_t i = 0; i < numbers.size(); ++i) {
        for (size_t j = 0; j < numbers.size(); ++j) {
            if (i != j) {
                max_magnitude = std::max(max_magnitude, (numbers[i] + numbers[j]).magnitude());
            }
        }
    }
    std::cout << max_magnitude << std::endl;
}
