#pragma once

struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const noexcept {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const noexcept {
        return !(*this == other);
    }

    Point& operator+=(const Point& other) noexcept {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point& operator-=(const Point& other) noexcept {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Point operator+(const Point& other) const noexcept {
        Point result = *this;
        result += other;
        return result;
    }

    Point operator-(const Point& other) const noexcept {
        Point result = *this;
        result -= other;
        return result;
    }
};

namespace std {
    template<>
    struct hash<Point> {
        size_t operator()(const Point& point) const noexcept {
            return static_cast<size_t>(point.x) + 31 * static_cast<size_t>(point.y);
        }
    };
}