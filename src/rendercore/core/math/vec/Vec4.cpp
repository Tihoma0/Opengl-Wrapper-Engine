#include "Vec4.h"
#include "../mat/Mat4.h"
#include <cmath>

#include "rendercore/surface/Color.h"

Vec4::Vec4() : x(0), y(0), z(0), w(0) {}

Vec4::Vec4(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w) {}

Vec4::Vec4(const Vec2 &v, float z, float w) : x(v.x), y(v.y), z(z), w(w) {}

Vec4::Vec4(const Vec3& v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

Vec4::Vec4(float v) : x(v), y(v), z(v), w(v) {}

Vec4::Vec4(const Color& color) : x(color.r), y(color.g), z(color.b), w(color.a) {}

Vec4 Vec4::operator+(const Vec4& other) const {
    return {x + other.x, y + other.y, z + other.z, w + other.w};
}

Vec4 Vec4::operator-(const Vec4& other) const {
    return {x - other.x, y - other.y, z - other.z, w - other.w};
}

Vec4 Vec4::operator*(float s) const {
    return {x * s, y * s, z * s, w * s};
}

Vec4 Vec4::operator/(float s) const {
    return {x / s, y / s, z / s, w / s};
}

Vec4& Vec4::operator+=(const Vec4& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Vec4& Vec4::operator-=(const Vec4& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

Vec4& Vec4::operator*=(float s) {
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
}

Vec4& Vec4::operator/=(float s) {
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
}

float Vec4::length() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

float Vec4::length2() const {
    return x * x + y * y + z * z + w * w;
}

Vec4 Vec4::normalize() const {
    const float len = length();

    if (len <= 0.0f) return {0,0,0,0};

    return {x / len, y / len, z / len, w / len};
}

float Vec4::dot(const Vec4& other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

float Vec4::distance2(const Vec4& other) const {
    const float dx = x - other.x;
    const float dy = y - other.y;
    const float dz = z - other.z;
    const float dw = w - other.w;

    return dx * dx + dy * dy + dz * dz + dw * dw;
}

float Vec4::distance(const Vec4& other) const {
    return std::sqrt(distance2(other));
}