#ifndef IVEC3_H
#define IVEC3_H

#include "IVec2.h"

class Color;

class IVec3 {
public:
    union {
        struct { int r, g, b; };
        struct { int x, y, z; };
        int m[3];
    };

    IVec3();
    IVec3(int x, int y, int z);
    explicit IVec3(const IVec2& v, int z = 0);
    explicit IVec3(int v);
    explicit IVec3(const Color& color);

    IVec3 operator+(const IVec3& other) const;
    IVec3 operator-(const IVec3& other) const;

    IVec3 operator*(int s) const;
    IVec3 operator/(int s) const;

    IVec3& operator+=(const IVec3& other);
    IVec3& operator-=(const IVec3& other);

    IVec3& operator*=(int s);
    IVec3& operator/=(int s);

    [[nodiscard]] float length() const;
    [[nodiscard]] int length2() const;

    [[nodiscard]] IVec3 normalize() const;

    [[nodiscard]] int dot(const IVec3& other) const;
    [[nodiscard]] IVec3 cross(const IVec3& other) const;

    [[nodiscard]] int distance2(const IVec3& other) const;
    [[nodiscard]] float distance(const IVec3& other) const;

    [[nodiscard]] float angle(const IVec3& other) const;

    [[nodiscard]] IVec2 xy() const { return {x, y}; }

    [[nodiscard]] std::string to_string() const {
        return "(" +
            std::to_string(x) + ", " +
            std::to_string(y) + ", " +
            std::to_string(z) + ")";
    }
};

#endif