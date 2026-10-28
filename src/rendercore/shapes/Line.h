#ifndef LINE_H
#define LINE_H
#include <algorithm>

#include "rendercore/core/math/vec/Vec2.h"

enum class IntersectionType
{
    None,
    Point,
    Overlap,
    Touching
};

struct Intersection
{
    bool does_intersect = false;
    IntersectionType type = IntersectionType::None;
    Vec2 point = {};
};

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
    [[nodiscard]] Intersection intersection(const Line& other, const float precision = 1e-6f) const {
        const float precision2 = precision * precision;
        const auto m_along = along();
        const auto o_along = other.along();
        float m_along_len = m_along.length();
        float o_along_len = o_along.length();

        const auto to_point = start() - other.start();

        const bool is_self_degenerate = m_along_len <= precision;
        const bool is_other_degenerate = o_along_len <= precision;
        if (is_self_degenerate && is_other_degenerate)
        {
            if (to_point.length2() <= precision2)
                return {.does_intersect = true, .type = IntersectionType::Touching, .point = start()};
            return {.does_intersect = false};
        }
        if (is_self_degenerate)
        {
            const auto projection_axis = o_along.normalized();

            if (std::abs(projection_axis.cross(to_point)) >= precision)
                return {.does_intersect = false};

            const float p = projection_axis.dot(to_point);
            if (p >= -precision && p <= o_along_len + precision)
                return {.does_intersect = true, .type = IntersectionType::Point, .point = start()};
            return {.does_intersect = false};
        }
        if (is_other_degenerate)
        {
            const auto projection_axis = m_along.normalized();

            if (std::abs(projection_axis.cross(to_point)) >= precision)
                return {.does_intersect = false};

            const float p = projection_axis.dot(to_point);
            if (p >= -precision && p <= m_along_len + precision)
                return {.does_intersect = true, .type = IntersectionType::Point, .point = other.start()};
            return {.does_intersect = false};
        }
        const float denom = m_along.cross(o_along);
        if (std::abs(denom) <= precision) {
            if (const auto to_other_start = other.start() - this->start(); std::abs(m_along.cross(to_other_start)) <= precision) {
                const auto projection_axis = m_along.normalized();
                const float p0 = start().dot(projection_axis);
                const float p1 = end().dot(projection_axis);

                const float p2 = other.start().dot(projection_axis);
                const float p3 = other.end().dot(projection_axis);

                const float max_own_point = std::max(p0, p1);
                const float min_own_point = std::min(p0, p1);

                const float max_other_point = std::max(p2, p3);
                const float min_other_point = std::min(p2, p3);

                const float overlap_start = std::max(min_own_point, min_other_point);
                const float overlap_end = std::min(max_own_point, max_other_point);

                if (overlap_start > overlap_end)
                {
                    return {.does_intersect = false};
                }
                const Vec2 intersection_point =
                    end() + projection_axis * ((overlap_start + overlap_end) * 0.5f - p1); // this is the result of a long dispute... and again I was wrong - but JUST leave me my own touch!

                const auto type = abs(overlap_start - overlap_end) <= precision
                    ? IntersectionType::Touching
                    : IntersectionType::Overlap;

                return {.does_intersect = true, .type = type, .point = intersection_point};
            }
            return {.does_intersect = false};
        }
        const Vec2 start_diff = other.start() - start();
        const float t_tolerance = (m_along_len > 0.0f) ? (precision / m_along_len) : 1e-4f;
        const float u_tolerance = (o_along_len > 0.0f) ? (precision / o_along_len) : 1e-4f;
        const float t = start_diff.cross(o_along) / denom;
        const float u = start_diff.cross(m_along) / denom;
        if (t >= -precision && t <= 1.0f + precision && u >= -precision && u <= 1.0f + precision) {
            IntersectionType type = t <= t_tolerance || t >= 1.0f - t_tolerance || u <= u_tolerance || u >= 1.0f - u_tolerance
                ? IntersectionType::Touching
                : IntersectionType::Overlap;
            Vec2 intersect_point = {
                this->x1 + t * m_along.x,
                this->y1 + t * m_along.y
            };
            return {.does_intersect = true, .type = type, .point = intersect_point};
        }
        return {.does_intersect = false};
    }
};



#endif //LINE_H
