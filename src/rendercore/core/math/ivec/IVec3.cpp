#include "IVec3.h"
#include <cmath>

#include "rendercore/surface/Color.h"

IVec3::IVec3() : x(0), y(0), z(0) {}

IVec3::IVec3(int x, int y, int z) : x(x), y(y), z(z) {}

IVec3::IVec3(int v) : x(v), y(v), z(v) {}

IVec3::IVec3(const IVec2& v, int z) : x(v.x), y(v.y), z(z) {}

IVec3::IVec3(const Color& color) : x(color.r), y(color.g), z(color.b) {}

IVec3 IVec3::operator+(const IVec3& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

IVec3 IVec3::operator-(const IVec3& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

IVec3 IVec3::operator*(int s) const {
    return {x * s, y * s, z * s};
}

IVec3 IVec3::operator/(int s) const {
    return {x / s, y / s, z / s};
}

IVec3& IVec3::operator+=(const IVec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

IVec3& IVec3::operator-=(const IVec3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

IVec3& IVec3::operator*=(int s) {
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

IVec3& IVec3::operator/=(int s) {
    x /= s;
    y /= s;
    z /= s;
    return *this;
}

float IVec3::length() const {
    return std::sqrt(x * x + y * y + z * z);
}

int IVec3::length2() const {
    return x * x + y * y + z * z;
}

IVec3 IVec3::normalize() const {
    const int len = length();
    if (len <= 0.0f) return {0, 0, 0};
    return {x / len, y / len, z / len};
}

int IVec3::dot(const IVec3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

IVec3 IVec3::cross(const IVec3& other) const {
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    };
}

int IVec3::distance2(const IVec3& other) const {
    const int dx = x - other.x;
    const int dy = y - other.y;
    const int dz = z - other.z;
    return dx * dx + dy * dy + dz * dz;
}

float IVec3::distance(const IVec3& other) const {
    return std::sqrt(distance2(other));
}

float IVec3::angle(const IVec3& other) const {
    const float denom = length() * other.length();

    if (denom <= 0.0f) return 0.0f;

    int cosTheta = dot(other) / denom;

    if (cosTheta > 1.0f) cosTheta = 1.0f;
    if (cosTheta < -1.0f) cosTheta = -1.0f;

    return std::acos(cosTheta);
}