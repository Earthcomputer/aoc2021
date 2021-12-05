#pragma once

struct Point {
    int x;
    int y;

    constexpr bool operator==(const Point& other) const noexcept {
        return x == other.x && y == other.y;
    }

    constexpr bool operator!=(const Point& other) const noexcept {
        return !(*this == other);
    }

    constexpr Point& operator+=(const Point& other) noexcept {
        x += other.x;
        y += other.y;
        return *this;
    }

    constexpr Point& operator-=(const Point& other) noexcept {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    constexpr Point operator+(const Point& other) const noexcept {
        Point result = *this;
        result += other;
        return result;
    }

    constexpr Point operator-(const Point& other) const noexcept {
        Point result = *this;
        result -= other;
        return result;
    }
};

namespace std {
    template<>
    struct hash<Point> {
        constexpr size_t operator()(const Point& point) const noexcept {
            return static_cast<size_t>(point.x) + 31 * static_cast<size_t>(point.y);
        }
    };
}