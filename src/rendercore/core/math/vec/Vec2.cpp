#include "Vec2.h"
#include <cmath>

#include "rendercore/surface/Color.h"

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2::Vec2(float v) : x(v), y(v) {}

Vec2::Vec2(const Color& color) : x(color.r), y(color.g) {}

Vec2 Vec2::operator+(const Vec2& other) const {
    return {x + other.x, y + other.y};
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return {x - other.x, y - other.y};
}

Vec2 Vec2::operator*(float s) const {
    return {x * s, y * s};
}

Vec2 Vec2::operator/(float s) const {
    return {x / s, y / s};
}

Vec2& Vec2::operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2& Vec2::operator*=(float s) {
    x *= s;
    y *= s;
    return *this;
}

Vec2& Vec2::operator/=(float s) {
    x /= s;
    y /= s;
    return *this;
}

float Vec2::length() const {
    return std::sqrt(x * x + y * y);
}

float Vec2::length2() const {
    return x * x + y * y;
}

Vec2 Vec2::normalize() const {
    const float len = length();
    if (len <= 0.0f) return {0, 0};
    return {x / len, y / len};
}

float Vec2::dot(const Vec2& other) const {
    return x * other.x + y * other.y;
}

float Vec2::distance2(const Vec2& other) const {
    const float dx = x - other.x;
    const float dy = y - other.y;
    return dx * dx + dy * dy;
}

float Vec2::distance(const Vec2& other) const {
    return std::sqrt(distance2(other));
}