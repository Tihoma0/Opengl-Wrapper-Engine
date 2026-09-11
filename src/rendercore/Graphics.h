
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <memory>

#include "core/math/shapes/CircleGroup.h"
#include "core/math/shapes/RectangleGroup.h"
#include "text/Text.h"


class Line;
class Rectangle;
class Circle;

namespace Graphics {
    void init();

    void draw(const std::shared_ptr<RenderTarget> &target, const Rectangle &rect, Color color);

    void draw(const std::shared_ptr<RenderTarget> &target, const Circle &circle, Color color);

    void draw(const std::shared_ptr<RenderTarget> &target, const Rectangle &rect, const std::shared_ptr<Texture2D> &texture);

    void draw(const std::shared_ptr<RenderTarget> &target, RectangleGroup &rects);

    void draw(const std::shared_ptr<RenderTarget> &target, CircleGroup &circles);

    void draw(const std::shared_ptr<RenderTarget> &target, const Text &text, const Vec2 &pos, const Color &color);

    void draw_line(const std::shared_ptr<RenderTarget> &target, Line &line, const Color &color);
};



#endif //GRAPHICS_H
