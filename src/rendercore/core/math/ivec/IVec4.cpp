#include "IVec4.h"
#include "../mat/Mat4.h"
#include <cmath>

#include "rendercore/surface/Color.h"

IVec4::IVec4() : x(0), y(0), z(0), w(0) {}

IVec4::IVec4(int x, int y, int z, int w)
    : x(x), y(y), z(z), w(w) {}

IVec4::IVec4(const IVec2 &v, int z, int w) : x(v.x), y(v.y), z(z), w(w) {}

IVec4::IVec4(const IVec3& v, int w) : x(v.x), y(v.y), z(v.z), w(w) {}

IVec4::IVec4(int v) : x(v), y(v), z(v), w(v) {}

IVec4::IVec4(const Color& color) : x(color.r), y(color.g), z(color.b), w(color.a) {}

IVec4 IVec4::operator+(const IVec4& other) const {
    return {x + other.x, y + other.y, z + other.z, w + other.w};
}

IVec4 IVec4::operator-(const IVec4& other) const {
    return {x - other.x, y - other.y, z - other.z, w - other.w};
}

IVec4 IVec4::operator*(int s) const {
    return {x * s, y * s, z * s, w * s};
}

IVec4 IVec4::operator/(int s) const {
    return {x / s, y / s, z / s, w / s};
}

IVec4& IVec4::operator+=(const IVec4& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

IVec4& IVec4::operator-=(const IVec4& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

IVec4& IVec4::operator*=(int s) {
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
}

IVec4& IVec4::operator/=(int s) {
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
}

float IVec4::length() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

int IVec4::length2() const {
    return x * x + y * y + z * z + w * w;
}

IVec4 IVec4::normalize() const {
    const int len = length();

    if (len <= 0.0f) return {0,0,0,0};

    return {x / len, y / len, z / len, w / len};
}

int IVec4::dot(const IVec4& other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

int IVec4::distance2(const IVec4& other) const {
    const int dx = x - other.x;
    const int dy = y - other.y;
    const int dz = z - other.z;
    const int dw = w - other.w;

    return dx * dx + dy * dy + dz * dz + dw * dw;
}

float IVec4::distance(const IVec4& other) const {
    return std::sqrt(distance2(other));
}