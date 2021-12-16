
#include <iostream>
#include "registration.h"

IMPLEMENT_DAY(16, Day16)

namespace {
    class PacketVisitor {
    public:
        virtual void visit_version(uint64_t version) {}
        virtual void visit_literal(uint64_t literal) {}
        virtual void visit_operator_start(uint64_t type_id) {}
        virtual void visit_operator_end() {}
    };

    class Day1Visitor : public PacketVisitor {
    public:
        uint64_t result = 0;
        void visit_version(uint64_t version) override {
            result += version;
        }
    };

    class Day2Visitor : public PacketVisitor {
        struct StackFrame {
            uint64_t operator_id;
            std::vector<uint64_t> results{};
        };
    public:
        uint64_t result{};
        std::vector<StackFrame> stack = {};

        void visit_literal(uint64_t literal) override {
            if (stack.empty()) {
                result = literal;
            } else {
                stack.back().results.push_back(literal);
            }
        }

        void visit_operator_start(uint64_t type_id) override {
            stack.push_back(StackFrame{type_id});
        }

        void visit_operator_end() override {
            const StackFrame frame = stack.back();
            stack.pop_back();
            uint64_t res;
            switch (frame.operator_id) {
                case 0: // sum
                res = 0;
                for (const uint64_t val : frame.results) {
                    res += val;
                }
                break;
                case 1: // product
                res = 1;
                for (const uint64_t val : frame.results) {
                    res *= val;
                }
                break;
                case 2: // minimum
                res = std::numeric_limits<uint64_t>::max();
                for (const uint64_t val : frame.results) {
                    res = std::min(val, res);
                }
                break;
                case 3: // maximum
                res = 0;
                for (const uint64_t val : frame.results) {
                    res = std::max(val, res);
                }
                break;
                case 5: // greater than
                res = frame.results[0] > frame.results[1] ? 1 : 0;
                break;
                case 6: // less than
                res = frame.results[0] < frame.results[1] ? 1 : 0;
                break;
                case 7: // equal to
                res = frame.results[0] == frame.results[1] ? 1 : 0;
                break;
                default: abort();
            }
            if (stack.empty()) {
                result = res;
            } else {
                stack.back().results.push_back(res);
            }
        }
    };

    bool next_bit(const std::string& input, size_t& pos) {
        const char c = input[pos >> 2];
        uint8_t val;
        if (c >= '0' && c <= '9') {
            val = c - '0';
        } else {
            val = c - 'A' + 10;
        }
        bool result = val & (1 << (3 - (pos & 3)));
        ++pos;
        return result;
    }

    uint64_t next_bits(const std::string& input, size_t& pos, int bits) {
        uint64_t result = 0;
        for (int i = 0; i < bits; ++i) {
            result = (result << 1) | (next_bit(input, pos) ? 1 : 0);
        }
        return result;
    }

    void parse_packet(const std::string& input, size_t& pos, PacketVisitor* visitor) {
        const auto version = next_bits(input, pos, 3);
        visitor->visit_version(version);
        const auto type_id = next_bits(input, pos, 3);
        if (type_id == 4) {
            uint64_t value = 0;
            uint64_t section;
            do {
                section = next_bits(input, pos, 5);
                value = (value << 4) | (section & 15);
            } while (section & 16);
            visitor->visit_literal(value);
        } else {
            visitor->visit_operator_start(type_id);
            const bool length_type_id = next_bit(input, pos);
            if (length_type_id) {
                const auto num_sub_packets = next_bits(input, pos, 11);
                for (uint64_t i = 0; i < num_sub_packets; ++i) {
                    parse_packet(input, pos, visitor);
                }
            } else {
                const auto num_bits = next_bits(input, pos, 15);
                const size_t start_pos = pos;
                while (pos - start_pos < num_bits) {
                    parse_packet(input, pos, visitor);
                }
            }
            visitor->visit_operator_end();
        }
    }

    void parse(const std::string& input, PacketVisitor* visitor) {
        size_t pos = 0;
        parse_packet(input, pos, visitor);
    }
}

void Day16::part1(const std::vector<std::string> &lines) const {
    Day1Visitor visitor;
    parse(lines[0], &visitor);
    std::cout << visitor.result << std::endl;
}

void Day16::part2(const std::vector<std::string> &lines) const {
    Day2Visitor visitor;
    parse(lines[0], &visitor);
    std::cout << visitor.result << std::endl;
}
