
#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <memory>

#include "core/math/CircleGroup.h"
#include "core/math/RectangleGroup.h"
#include "mesh/Mesh.h"
#include "render/Framebuffer.h"
#include "render/Renderer.h"


class Rectangle;
class Circle;

class Graphics {
public:
    static void init();

    static void draw_rectangle(const std::shared_ptr<RenderTarget> &target, const Rectangle &rect, Color color);

    static void draw_circle(const std::shared_ptr<RenderTarget> &target, const Circle &circle, Color color);

    static void draw_texture(const std::shared_ptr<RenderTarget> &target, const Rectangle &rect, const std::shared_ptr<Texture2D> &texture);

    static void draw_rectangle_group(const std::shared_ptr<RenderTarget> &target, RectangleGroup &rects, Color color);

    static void draw_circle_group(const std::shared_ptr<RenderTarget> &target, CircleGroup &circles, Color color);
private:
    static thread_local bool is_init;
    static std::shared_ptr<Mesh> quad;
    static thread_local std::unique_ptr<Material> rectangle_material;
    static thread_local std::unique_ptr<Material> circle_material;
    static thread_local std::unique_ptr<Material> texture_material;
    static thread_local std::unique_ptr<Material> rectangle_group_material;
    static thread_local std::unique_ptr<Material> circle_group_material;
};



#endif //GRAPHICS_H
