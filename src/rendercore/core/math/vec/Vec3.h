#ifndef VEC3_H
#define VEC3_H

#include "Vec2.h"

class Color;

class Vec3 {
public:
    union {
        struct { float x, y, z; };
        float m[3];
        struct { float r, g, b; };
    };

    Vec3();
    Vec3(float x, float y, float z);
    explicit Vec3(const Vec2& v, float z = 0);
    explicit Vec3(float v);
    explicit Vec3(const Color& color);

    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;

    Vec3 operator*(float s) const;
    Vec3 operator/(float s) const;

    Vec3& operator+=(const Vec3& other);
    Vec3& operator-=(const Vec3& other);

    Vec3& operator*=(float s);
    Vec3& operator/=(float s);

    [[nodiscard]] float length() const;
    [[nodiscard]] float length2() const;

    [[nodiscard]] Vec3 normalize() const;

    [[nodiscard]] float dot(const Vec3& other) const;
    [[nodiscard]] Vec3 cross(const Vec3& other) const;

    [[nodiscard]] float distance2(const Vec3& other) const;
    [[nodiscard]] float distance(const Vec3& other) const;

    [[nodiscard]] float angle(const Vec3& other) const;

    [[nodiscard]] Vec2 xy() const { return {x, y}; }

    [[nodiscard]] std::string to_string() const {
        return "(" +
            std::to_string(x) + ", " +
            std::to_string(y) + ", " +
            std::to_string(z) + ")";
    }
};

#endif