#include "Mat3.h"
#include <algorithm>

Mat3::Mat3() : m{0} {}

Mat3::Mat3(const float values[9]) {
    std::copy_n(values, 9, m);
}

Mat3 Mat3::Identity() {
    float v[9] = {
        1,0,0,
        0,1,0,
        0,0,1
    };
    return Mat3(v);
}

Mat3 Mat3::operator*(const Mat3& b) const {
    Mat3 r;

    for (int col = 0; col < 3; ++col) {
        for (int row = 0; row < 3; ++row) {
            r.m[col * 3 + row] =
                m[0 * 3 + row] * b.m[col * 3 + 0] +
                m[1 * 3 + row] * b.m[col * 3 + 1] +
                m[2 * 3 + row] * b.m[col * 3 + 2];
        }
    }

    return r;
}

Mat3& Mat3::operator*=(const Mat3& b) {
    *this = *this * b;
    return *this;
}

Vec3 Mat3::operator*(const Vec3& v) const {
    Vec3 r;

    r.x = m[0] * v.x + m[3] * v.y + m[6] * v.z;
    r.y = m[1] * v.x + m[4] * v.y + m[7] * v.z;
    r.z = m[2] * v.x + m[5] * v.y + m[8] * v.z;

    return r;
}

Mat3 Mat3::operator*(float s) const {
    Mat3 r;
    for (int i = 0; i < 9; ++i) r.m[i] = m[i] * s;
    return r;
}

Mat3& Mat3::operator*=(float s) {
    for (int i = 0; i < 9; ++i) m[i] *= s;
    return *this;
}