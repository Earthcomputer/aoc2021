#pragma once

#include <cmath>
#include <type_traits>
#include <unordered_set>

template<class T>
class Vec2 {
public:
    T x;
    T y;

    constexpr Vec2() noexcept(noexcept(T())) : x(), y() {}
    constexpr Vec2(const T& x, const T& y) noexcept(noexcept(T(std::declval<T>()))) : x(x), y(y) {}
    constexpr Vec2(T&& x, T&& y) noexcept : x(std::move(x)), y(std::move(y)) {}
    template<class U>
    explicit(!std::is_convertible_v<U, T>) constexpr Vec2(const Vec2<U>& point) noexcept(noexcept(T(std::declval<U>()))) : x(point.x), y(point.y) {}

    template<class U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
    constexpr Vec2<T>& operator=(const Vec2<U>& other) noexcept(noexcept((std::declval<T>() = std::declval<U>()))) {
        x = other.x;
        y = other.y;
        return *this;
    }

    template<class U>
    constexpr bool operator==(const Vec2<U>& other) const noexcept(noexcept(std::declval<T>() == std::declval<U>())) {
        return x == other.x && y == other.y;
    }

    template<class U>
    constexpr bool operator!=(const Vec2<U>& other) const noexcept(noexcept(std::declval<T>() != std::declval<U>())) {
        return x != other.x || y != other.y;
    }

    template<class U>
    constexpr Vec2<T>& operator+=(const Vec2<U>& other) noexcept(noexcept((std::declval<T>() += std::declval<U>()))) {
        x += other.x;
        y += other.y;
        return *this;
    }

    template<class U>
    constexpr Vec2<T>& operator-=(const Vec2<U>& other) noexcept(noexcept((std::declval<T>() -= std::declval<U>()))) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    template<class U>
    constexpr auto operator+(const Vec2<U>& other) const noexcept(noexcept(std::declval<T>() + std::declval<U>())) {
        Vec2<decltype(this->x + other.x)> result;
        result.x = this->x + other.x;
        result.y = this->y + other.y;
        return result;
    }

    template<class U>
    constexpr auto operator-(const Vec2<U>& other) const noexcept(noexcept(std::declval<T>() - std::declval<U>())) {
        Vec2<decltype(this->x - other.x)> result;
        result.x = this->x - other.x;
        result.y = this->y - other.y;
        return result;
    }

    constexpr auto operator-() const noexcept(noexcept(-std::declval<T>())) {
        Vec2<decltype(-this->x)> result;
        result.x = -this->x;
        result.y = -this->y;
        return result;
    }

    template<class U>
    constexpr auto operator*(const U& multiplier) const noexcept(noexcept(std::declval<T>() * std::declval<U>())) {
        Vec2<decltype(this->x * multiplier)> result;
        result.x = this->x * multiplier;
        result.y = this->y * multiplier;
        return result;
    }

    template<class U>
    constexpr auto operator/(const U& divisor) const noexcept(noexcept(std::declval<T>() / std::declval<U>())) {
        Vec2<decltype(this->x / divisor)> result;
        result.x = this->x / divisor;
        result.y = this->y / divisor;
        return result;
    }

    template<class U>
    constexpr Vec2<T>& operator*=(const U& multiplier) noexcept(noexcept((std::declval<T>() *= std::declval<U>()))) {
        x *= multiplier;
        y *= multiplier;
        return *this;
    }

    template<class U>
    constexpr Vec2<T>& operator/=(const U& divisor) noexcept(noexcept((std::declval<T>() /= std::declval<U>()))) {
        x /= divisor;
        y /= divisor;
        return *this;
    }

    constexpr T size_squared() const noexcept(noexcept(std::declval<T>() * std::declval<T>()) && noexcept(std::declval<T>() + std::declval<T>())) {
        return x * x + y * y;
    }

    template<class U = double>
    U size() const {
        return std::sqrt(static_cast<U>(size_squared()));
    }

    template<class U = double>
    Vec2<U> normalized() const {
        return *this / size<U>();
    }

    template<class U>
    constexpr auto operator|(const Vec2<U>& other) const noexcept(noexcept(std::declval<Vec2<T>>().size_squared())) {
        return x * other.x + y * other.y;
    }

    template<class U>
    constexpr auto distance_squared(const Vec2<U>& other) const noexcept(noexcept((std::declval<Vec2<T>>() - std::declval<Vec2<U>>()).size_squared())) {
        return (*this - other).size_squared();
    }

    template<class U, class V = double>
    V distance(const Vec2<U>& other) const {
        return std::sqrt(static_cast<V>(distance_squared(other)));
    }
};

template<class T, class U>
constexpr auto operator*(const T& multiplier, const Vec2<U>& point) noexcept(noexcept(std::declval<T>() * std::declval<U>())) {
    Vec2<decltype(multiplier * point.x)> result;
    result.x = multiplier * point.x;
    result.y = multiplier * point.y;
    return result;
}

using Point = Vec2<int>;

namespace std {
    template<class T>
    struct hash<Vec2<T>> {
        constexpr size_t operator()(const Vec2<T>& point) const noexcept(noexcept(std::hash<T>{}(std::declval<T>()))) {
            return std::hash<T>{}(point.x) + 31 * std::hash<T>{}(point.y);
        }
    };
}