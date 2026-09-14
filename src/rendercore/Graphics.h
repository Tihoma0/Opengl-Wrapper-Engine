
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <memory>

#include "core/math/shapes/BezierGroup.h"


class BezierCurve;
class Vec2;
class CircleGroup;
class RectangleGroup;
class Color;
class Texture2D;
class RenderTarget;
class Text;
class LineGroup;
class Line;
class Rectangle;
class Circle;

namespace Graphics {
    void init();
    void init_rectangle();
    void init_rectangle_group();
    void init_line();
    void init_line_group();
    void init_circle();
    void init_circle_group();
    void init_texture();
    void init_simple_line();
    void init_simple_line_group();
    void init_bezier();
    void init_bezier_group();


    void draw(const std::shared_ptr<RenderTarget> &target, const Rectangle &rect, const Color &color);

    void draw(const std::shared_ptr<RenderTarget> &target, const Circle &circle, const Color &color);

    void draw(const std::shared_ptr<RenderTarget> &target, const Rectangle &rect, const std::shared_ptr<Texture2D> &texture);

    void draw(const std::shared_ptr<RenderTarget> &target, RectangleGroup &rects);

    void draw(const std::shared_ptr<RenderTarget> &target, CircleGroup &circles);

    void draw(const std::shared_ptr<RenderTarget> &target, const Text &text, const Vec2 &pos, const Color &color);




    void draw_line(const std::shared_ptr<RenderTarget> &target, BezierCurve &curve, const Color &color, int num_segments = 10);

    void draw_lines(const std::shared_ptr<RenderTarget> &target, BezierGroup &group, int num_segments = 10);

    void draw(const std::shared_ptr<RenderTarget> &target, const Line &line, const Color &color);

    void draw(const std::shared_ptr<RenderTarget> &target, LineGroup &lines);

    void draw_line(const std::shared_ptr<RenderTarget> &target, const Line &line, const Color &color);

    void draw_lines(const std::shared_ptr<RenderTarget> &target, LineGroup &lines, float width);

};






#endif //GRAPHICS_H
