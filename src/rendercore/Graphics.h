
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <memory>

#include "core/math/CircleGroup.h"
#include "core/math/RectangleGroup.h"
#include "render/Framebuffer.h"
#include "text/Text.h"


class Rectangle;
class Circle;

namespace Graphics {
    void init();

    void draw(const std::shared_ptr<RenderTarget> &target, const Rectangle &rect, Color color);

    void draw(const std::shared_ptr<RenderTarget> &target, const Circle &circle, Color color);

    void draw(const std::shared_ptr<RenderTarget> &target, const Rectangle &rect, const std::shared_ptr<Texture2D> &texture);

    void draw(const std::shared_ptr<RenderTarget> &target, RectangleGroup &rects);

    void draw(const std::shared_ptr<RenderTarget> &target, CircleGroup &circles);

    void draw(const std::shared_ptr<RenderTarget> &target, const Text &text, Vec2 pos);
};



#endif //GRAPHICS_H
