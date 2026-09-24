#ifndef VEC2_H
#define VEC2_H
#include <cmath>
#include <string>

class Color;

class Vec2 {
public:
    union {
        struct { float x, y; };
        float vert[2];
        struct { float r, g; };
    };

    Vec2();
    Vec2(float x, float y);
    explicit Vec2(float v);
    explicit Vec2(const Color& color);

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;

    Vec2 operator*(float s) const;
    Vec2 operator/(float s) const;

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);

    Vec2& operator*=(float s);
    Vec2& operator/=(float s);

    [[nodiscard]] float length() const;
    [[nodiscard]] float length2() const;

    [[nodiscard]] Vec2 normalize() const;

    [[nodiscard]] float dot(const Vec2& other) const;
    [[nodiscard]] float cross(const Vec2& other) const;

    [[nodiscard]] float distance2(const Vec2& other) const;
    [[nodiscard]] float distance(const Vec2& other) const;

    [[nodiscard]] float m() const {
        constexpr float epsilon = 1e-6f;
        if (std::abs(x) < epsilon)
            return (y / epsilon) * (std::signbit(x) ? 1.0f : -1.0f);
        return y / x;
    }

    [[nodiscard]] std::string to_string() const {
        return "(" +
            std::to_string(x) + ", " +
            std::to_string(y) + ")";
    }
};

#endif