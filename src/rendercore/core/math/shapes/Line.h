#ifndef LINE_H
#define LINE_H
#include "../vec/Vec2.h"

class Line {
public:
    float x1, x2, y1, y2, width;

    Line(const float x1, const float y1, const float x2, const float y2, const float width = 1.0f) : x1(x1), x2(x2), y1(y1), y2(y2), width(width) {}

    [[nodiscard]] Vec2 start() const { return {x1, y1}; }
    [[nodiscard]] Vec2 end() const { return {x2, y2}; }
    [[nodiscard]] Vec2 center() const { return {(x1 + x2) / 2, (y1 + y2) / 2}; }
    [[nodiscard]] Vec2 vector() const { return {x2 - x1, y2 - y1}; }
};



#endif //LINE_H
