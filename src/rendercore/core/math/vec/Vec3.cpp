#include "Vec3.h"
#include <cmath>

#include "rendercore/surface/Color.h"

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

Vec3::Vec3(float v) : x(v), y(v), z(v) {}

Vec3::Vec3(const Vec2& v, float z) : x(v.x), y(v.y), z(z) {}

Vec3::Vec3(const Color& color) : x(color.r), y(color.g), z(color.b) {}

Vec3 Vec3::operator+(const Vec3& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

Vec3 Vec3::operator*(float s) const {
    return {x * s, y * s, z * s};
}

Vec3 Vec3::operator/(float s) const {
    return {x / s, y / s, z / s};
}

Vec3& Vec3::operator+=(const Vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vec3& Vec3::operator*=(float s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

Vec3& Vec3::operator/=(float s) {
    x /= s;
    y /= s;
    z /= s;
    return *this;
}

float Vec3::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

float Vec3::length2() const {
    return x * x + y * y + z * z;
}

Vec3 Vec3::normalize() const {
    const float len = length();
    if (len <= 0.0f) return {0, 0, 0};
    return {x / len, y / len, z / len};
}

float Vec3::dot(const Vec3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3& other) const {
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

float Vec3::distance2(const Vec3& other) const {
    const float dx = x - other.x;
    const float dy = y - other.y;
    const float dz = z - other.z;
    return dx * dx + dy * dy + dz * dz;
}

float Vec3::distance(const Vec3& other) const {
    return std::sqrt(distance2(other));
}

float Vec3::angle(const Vec3& other) const {
    const float denom = length() * other.length();

    if (denom <= 0.0f) return 0.0f;

    float cosTheta = dot(other) / denom;

    if (cosTheta > 1.0f) cosTheta = 1.0f;
    if (cosTheta < -1.0f) cosTheta = -1.0f;

    return std::acos(cosTheta);
}