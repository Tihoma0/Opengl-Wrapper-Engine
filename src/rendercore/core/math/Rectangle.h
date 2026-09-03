#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "vec/Vec2.h"


class Rectangle {
public:
    Rectangle(const float x, const float y, const float w, const float h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    float x, y, w, h;

    [[nodiscard]] float top() const { return y + h; }
    [[nodiscard]] float bottom() const { return y; }
    [[nodiscard]] float left() const { return x; }
    [[nodiscard]] float right() const { return x + w; }

    [[nodiscard]] Vec2  position() const { return {x, y}; }
    [[nodiscard]] Vec2  size() const { return {w, h}; }

};



#endif //RECTANGLE_H
