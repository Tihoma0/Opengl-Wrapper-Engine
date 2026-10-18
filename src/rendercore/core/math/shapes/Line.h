#ifndef LINE_H
#define LINE_H
#include "../vec/Vec2.h"

class Line {
public:
    float x1, x2, y1, y2, width;

    Line() = default;

    Line(const Vec2& start, const Vec2& end, const float width = 1.0f) : x1(start.x), x2(end.x), y1(start.y), y2(end.y), width(width) {}

    Line(const float x1, const float y1, const float x2, const float y2, const float width = 1.0f) : x1(x1), x2(x2), y1(y1), y2(y2), width(width) {}

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
            const auto to_other_start = other.start() - this->start();
            if (std::abs(m_along.cross(to_other_start)) < 1e-6f) {
                // THEY ARE COLLINEAR!
                // Depending on your logic, a hole vertex touching an edge
                // means you should return 'true' and the overlapping vertex coordinate.
                // (You will need boundary-box checks here to see if they actually overlap)
            }
            return {false, {0.0f, 0.0f}};
        }
        const Vec2 start_diff = other.start() - start();
        const float t = start_diff.cross(o_along) / denom;
        const float u = start_diff.cross(m_along) / denom;
        if (t >= -1e-6f && t <= 1.0f + 1e-6f && u >= -1e-6f && u <= 1.0f + 1e-6f) {
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
