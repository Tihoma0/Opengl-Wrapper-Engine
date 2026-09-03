#ifndef IVEC4_H
#define IVEC4_H

#include "IVec3.h"

class Color;
class Mat4;

class IVec4 {
public:
    union {
        struct { int r, g, b, a; };
        struct { int x, y, z, w; };
        int m[4];
    };

    IVec4();
    IVec4(int x, int y, int z, int w);
    explicit IVec4(int v);
    explicit IVec4(const IVec2& v, int z = 0, int w = 0);
    explicit IVec4(const IVec3& v, int w = 0);
    explicit IVec4(const Color& color);

    IVec4 operator+(const IVec4& other) const;
    IVec4 operator-(const IVec4& other) const;

    IVec4 operator*(int s) const;
    IVec4 operator/(int s) const;

    IVec4& operator+=(const IVec4& other);
    IVec4& operator-=(const IVec4& other);

    IVec4& operator*=(int s);
    IVec4& operator/=(int s);

    [[nodiscard]] float length() const;
    [[nodiscard]] int length2() const;

    [[nodiscard]] IVec4 normalize() const;

    [[nodiscard]] int dot(const IVec4& other) const;

    [[nodiscard]] int distance2(const IVec4& other) const;
    [[nodiscard]] float distance(const IVec4& other) const;

    [[nodiscard]] IVec3 xyz() const {
        return {x, y, z};
    }

    [[nodiscard]] IVec2 xy() const {
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