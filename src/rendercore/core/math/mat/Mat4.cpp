#include "Mat4.h"
#include <algorithm>

Mat4::Mat4() : m{0} {}

Mat4::Mat4(const float values[16]) {
    std::copy_n(values, 16, m);
}

Mat4 Mat4::Identity() {
    float values[] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    return Mat4(values);
}

Mat4 Mat4::operator*(const Mat4& b) const {
    Mat4 r;

    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            r.m[col * 4 + row] =
                m[0 * 4 + row] * b.m[col * 4 + 0] +
                m[1 * 4 + row] * b.m[col * 4 + 1] +
                m[2 * 4 + row] * b.m[col * 4 + 2] +
                m[3 * 4 + row] * b.m[col * 4 + 3];
        }
    }

    return r;
}

Mat4& Mat4::operator*=(const Mat4& b) {
    *this = *this * b;
    return *this;
}

Vec4 Mat4::operator*(const Vec4& v) const {
    Vec4 r;

    r.x =
        m[0] * v.x +
        m[4] * v.y +
        m[8] * v.z +
        m[12] * v.w;

    r.y =
        m[1] * v.x +
        m[5] * v.y +
        m[9] * v.z +
        m[13] * v.w;

    r.z =
        m[2] * v.x +
        m[6] * v.y +
        m[10] * v.z +
        m[14] * v.w;

    r.w =
        m[3] * v.x +
        m[7] * v.y +
        m[11] * v.z +
        m[15] * v.w;

    return r;
}

Mat4 Mat4::operator*(float s) const {
    Mat4 r;

    for (int i = 0; i < 16; ++i) {
        r.m[i] = m[i] * s;
    }

    return r;
}

Mat4& Mat4::operator*=(float s) {
    for (int i = 0; i < 16; ++i) {
        m[i] *= s;
    }

    return *this;
}