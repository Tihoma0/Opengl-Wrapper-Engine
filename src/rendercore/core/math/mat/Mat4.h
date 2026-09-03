#ifndef MAT4_H
#define MAT4_H

#include <cmath>

#include "../vec/Vec4.h"

class Mat4 {
public:
    float m[16];

    Mat4();
    explicit Mat4(const float values[16]);

    static Mat4 Identity();
    static Mat4 Translate(const Vec3& v) {
        Mat4 r = Identity();

        r.m[12] = v.x;
        r.m[13] = v.y;
        r.m[14] = v.z;

        return r;
    }

    static Mat4 Scale(const Vec3& v) {
        Mat4 r = Identity();

        r.m[0] = v.x;
        r.m[5] = v.y;
        r.m[10] = v.z;

        return r;
    }

    static Mat4 RotateX(float angle) {
        Mat4 r = Identity();

        float c = std::cos(angle);
        float s = std::sin(angle);

        r.m[5] = c;
        r.m[9] = -s;

        r.m[6] = s;
        r.m[10] = c;

        return r;
    }

    static Mat4 RotateY(float angle) {
        Mat4 r = Identity();

        float c = std::cos(angle);
        float s = std::sin(angle);

        r.m[0] = c;
        r.m[8] = s;

        r.m[2] = -s;
        r.m[10] = c;

        return r;
    }

    static Mat4 RotateZ(float angle) {
        Mat4 r = Identity();

        float c = std::cos(angle);
        float s = std::sin(angle);

        r.m[0] = c;
        r.m[4] = -s;

        r.m[1] = s;
        r.m[5] = c;

        return r;
    }

    static Mat4 Orthographic(
    float left,
    float right,
    float bottom,
    float top,
    float near_plane,
    float far_plane
    ) {
        Mat4 r = Identity();

        r.m[0] = 2.0f / (right - left);
        r.m[5] = 2.0f / (top - bottom);
        r.m[10] = -2.0f / (far_plane - near_plane);

        r.m[12] = -(right + left) / (right - left);
        r.m[13] = -(top + bottom) / (top - bottom);
        r.m[14] = -(far_plane + near_plane) / (far_plane - near_plane);

        return r;
    }

    static Mat4 Perspective(
    float fov,
    float aspect,
    float near_plane,
    float far_plane
    ) {
        Mat4 r{};

        float tanHalf = std::tan(fov / 2.0f);

        r.m[0] = 1.0f / (aspect * tanHalf);
        r.m[5] = 1.0f / tanHalf;

        r.m[10] = -(far_plane + near_plane) / (far_plane - near_plane);
        r.m[11] = -1.0f;

        r.m[14] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);

        return r;
    }

    static Mat4 LookAt(
    const Vec3& eye,
    const Vec3& center,
    const Vec3& up
    ) {
        Vec3 f = (center - eye).normalize();
        Vec3 s = f.cross(up).normalize();
        Vec3 u = s.cross(f);

        Mat4 r = Identity();

        r.m[0] = s.x;
        r.m[1] = u.x;
        r.m[2] = -f.x;

        r.m[4] = s.y;
        r.m[5] = u.y;
        r.m[6] = -f.y;

        r.m[8] = s.z;
        r.m[9] = u.z;
        r.m[10] = -f.z;

        r.m[12] = -s.dot(eye);
        r.m[13] = -u.dot(eye);
        r.m[14] = f.dot(eye);

        return r;
    }


    Mat4 operator*(const Mat4& other) const;
    Mat4& operator*=(const Mat4& other);

    Vec4 operator*(const Vec4& v) const;

    Mat4 operator*(float s) const;
    Mat4& operator*=(float s);

    [[nodiscard]] std::string to_string() const {
        std::string s = "[\n";

        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                s += std::to_string(m[col * 4 + row]);
                if (col < 3) s += ", ";
            }
            s += "\n";
        }
        s += "]";

        return s;
    }
};

#endif