#ifndef IVEC2_H
#define IVEC2_H
#include <string>

class Vec2;

class Color;

class IVec2 {
public:
    union {
        struct { int r, g; };
        struct { int x, y; };
        int m[2];
    };

    IVec2();
    IVec2(int x, int y);
    explicit IVec2(int v);
    explicit IVec2(const Color& color);

    IVec2 operator+(const IVec2& other) const;
    IVec2 operator-(const IVec2& other) const;

    IVec2 operator*(int s) const;
    IVec2 operator/(int s) const;

    IVec2& operator+=(const IVec2& other);
    IVec2& operator-=(const IVec2& other);

    IVec2& operator*=(int s);
    IVec2& operator/=(int s);

    [[nodiscard]] float length() const;
    [[nodiscard]] int length2() const;

    [[nodiscard]] Vec2 normalize() const;

    [[nodiscard]] int dot(const IVec2& other) const;

    [[nodiscard]] int distance2(const IVec2& other) const;
    [[nodiscard]] float distance(const IVec2& other) const;

    [[nodiscard]] std::string to_string() const {
        return "(" +
            std::to_string(x) + ", " +
            std::to_string(y) + ")";
    }
};

#endif