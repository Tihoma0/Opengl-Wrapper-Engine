#ifndef LINE_H
#define LINE_H
#include "../vec/Vec2.h"

class Line {
public:
    float x1, x2, y1, y2, width;

    Line() = default;

    Line(const float x1, const float y1, const float x2, const float y2, const float width = 10.0f) : x1(x1), x2(x2), y1(y1), y2(y2), width(width) {}

    [[nodiscard]] Vec2 start() const { return {x1, y1}; }
    [[nodiscard]] Vec2 end() const { return {x2, y2}; }
    [[nodiscard]] Vec2 center() const { return {(x1 + x2) / 2, (y1 + y2) / 2}; }
    [[nodiscard]] Vec2 along() const { return {x2 - x1, y2 - y1}; }
    [[nodiscard]] Line swap() const { return {x2, y2, x1, y1}; }
    [[nodiscard]] std::pair<bool, Vec2> intersection(const Line& other) const {
        const auto m_along = along();
        const auto o_along = other.along();
        const float denom = m_along.cross(o_along);
        if (std::abs(denom) < 1e-6f) {
            return {false, {0.0f, 0.0f}};
        }
        const Vec2 start_diff = other.start() - start();
        const float t = start_diff.cross(o_along) / denom;
        const float u = start_diff.cross(m_along) / denom;
        if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
            Vec2 intersect_point = {
                this->x1 + t * m_along.x,
                this->y1 + t * m_along.y
            };
            return {true, intersect_point};
        }
        return {false, {0.0f, 0.0f}};
    }
};



#endif //LINE_H
