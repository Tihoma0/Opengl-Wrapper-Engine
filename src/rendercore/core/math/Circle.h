#ifndef CIRCLE_H
#define CIRCLE_H
#include "vec/Vec2.h"


class Circle {
public:
    float x, y, radius;
    Circle(float x, float y, float radius) : x(x), y(y), radius(radius) {}

    [[nodiscard]] Vec2 position() const { return {x, y}; }
};



#endif //CIRCLE_H
