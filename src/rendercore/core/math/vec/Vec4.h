#ifndef VEC4_H
#define VEC4_H

#include "Vec3.h"

class Color;
class Mat4;

class Vec4 {
public:
    union {
        struct { float x, y, z, w; };
        float m[4];
        struct { float r, g, b, a; };
    };

    Vec4();
    Vec4(float x, float y, float z, float w);
    explicit Vec4(float v);
    explicit Vec4(const Vec2& v, float z = 0, float w = 0);
    explicit Vec4(const Vec3& v, float w = 0);
    explicit Vec4(const Color& color);

    Vec4 operator+(const Vec4& other) const;
    Vec4 operator-(const Vec4& other) const;

    Vec4 operator*(float s) const;
    Vec4 operator/(float s) const;

    Vec4& operator+=(const Vec4& other);
    Vec4& operator-=(const Vec4& other);

    Vec4& operator*=(float s);
    Vec4& operator/=(float s);

    [[nodiscard]] float length() const;
    [[nodiscard]] float length2() const;

    [[nodiscard]] Vec4 normalize() const;

    [[nodiscard]] float dot(const Vec4& other) const;

    [[nodiscard]] float distance2(const Vec4& other) const;
    [[nodiscard]] float distance(const Vec4& other) const;

    [[nodiscard]] Vec3 xyz() const {
        return {x, y, z};
    }

    [[nodiscard]] Vec2 xy() const {
        return {x, y};
    }

    [[nodiscard]] std::string to_string() const {
        return "(" +
            std::to_string(x) + ", " +
            std::to_string(y) + ", " +
            std::to_string(z) + ", " +
            std::to_string(w) + ")";
    }
};

#endif