#include "IVec2.h"
#include <cmath>

#include "rendercore/core/math/vec/Vec2.h"
#include "rendercore/surface/Color.h"

IVec2::IVec2() : x(0), y(0) {}

IVec2::IVec2(int x, int y) : x(x), y(y) {}

IVec2::IVec2(int v) : x(v), y(v) {}

IVec2::IVec2(const Color& color) : x(color.r), y(color.g) {}

IVec2 IVec2::operator+(const IVec2& other) const {
    return {x + other.x, y + other.y};
}

IVec2 IVec2::operator-(const IVec2& other) const {
    return {x - other.x, y - other.y};
}

IVec2 IVec2::operator*(int s) const {
    return {x * s, y * s};
}

IVec2 IVec2::operator/(int s) const {
    return {x / s, y / s};
}

IVec2& IVec2::operator+=(const IVec2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

IVec2& IVec2::operator-=(const IVec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

IVec2& IVec2::operator*=(int s) {
    x *= s;
    y *= s;
    return *this;
}

IVec2& IVec2::operator/=(int s) {
    x /= s;
    y /= s;
    return *this;
}

float IVec2::length() const {
    return std::sqrt(x * x + y * y);
}

int IVec2::length2() const {
    return x * x + y * y;
}

Vec2 IVec2::normalize() const {
    const float len = length();
    if (len <= 0.0f) return {0, 0};
    return Vec2(x / len, y / len);
}

int IVec2::dot(const IVec2& other) const {
    return x * other.x + y * other.y;
}

int IVec2::distance2(const IVec2& other) const {
    const int dx = x - other.x;
    const int dy = y - other.y;
    return dx * dx + dy * dy;
}

float IVec2::distance(const IVec2& other) const {
    return std::sqrt(distance2(other));
}