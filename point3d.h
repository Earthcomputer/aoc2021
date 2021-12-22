#pragma once

#include <unordered_set>

class Point3d {
public:
    int x;
    int y;
    int z;

    constexpr Point3d() noexcept: x(0), y(0), z(0) {}

    constexpr Point3d(int x, int y, int z) noexcept: x(x), y(y), z(z) {}

    constexpr bool operator==(const Point3d &other) const noexcept {
        return x == other.x && y == other.y && z == other.z;
    }

    constexpr bool operator!=(const Point3d &other) const noexcept {
        return !(*this == other);
    }

    constexpr Point3d operator+(const Point3d& other) const noexcept {
        return {x + other.x, y + other.y, z + other.z};
    }

    constexpr Point3d operator-(const Point3d& other) const noexcept {
        return {x - other.x, y - other.y, z - other.z};
    }
};

std::ostream& operator<<(std::ostream& os, const Point3d& point);

namespace std {
    template<>
    struct hash<Point3d> {
        size_t operator()(const Point3d& point) const noexcept {
            return 961 * point.x + 31 * point.y + point.z;
        }
    };
}
