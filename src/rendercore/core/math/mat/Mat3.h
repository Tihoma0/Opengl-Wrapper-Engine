#ifndef MAT3_H
#define MAT3_H

#include "../vec/Vec3.h"

class Mat3 {
public:
    float m[9]; // column-major (3x3)

    Mat3();
    explicit Mat3(const float values[9]);

    static Mat3 Identity();

    Mat3 operator*(const Mat3& other) const;
    Mat3& operator*=(const Mat3& other);

    Vec3 operator*(const Vec3& v) const;

    Mat3 operator*(float s) const;
    Mat3& operator*=(float s);

    [[nodiscard]] std::string to_string() const {
        std::string s = "[\n";

        for (int row = 0; row < 3; ++row) {
            s += "";
            for (int col = 0; col < 3; ++col) {
                s += std::to_string(m[col * 3 + row]);
                if (col < 2) s += ", ";
            }
            s += "\n";
        }
        s += "]";

        return s;
    }
};

#endif