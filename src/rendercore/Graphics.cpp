
#include "Graphics.h"

#include "material/Material.h"
#include "material/Shader.h"
#include "mesh/Mesh.h"
#include "rendercore/core/enums/mesh/DrawOptions.h"
#include "core/math/shapes/Rectangle.h"
#include "core/math/shapes/Circle.h"
#include "core/math/shapes/Line.h"
#include "render/Renderer.h"

thread_local std::unique_ptr<Material> rectangle_material;
thread_local std::unique_ptr<Material> circle_material;
thread_local std::unique_ptr<Material> line_material;
thread_local std::unique_ptr<Material> simple_line_material;
thread_local std::unique_ptr<Material> texture_material;
thread_local std::unique_ptr<Material> rectangle_group_material;
thread_local std::unique_ptr<Material> circle_group_material;
thread_local std::unique_ptr<Material> line_group_material;
thread_local std::unique_ptr<Material> simple_line_group_material;

thread_local bool is_init = false;
thread_local std::shared_ptr<Mesh> quad;
thread_local std::shared_ptr<Mesh> simple_line;

void Graphics::draw(const std::shared_ptr<RenderTarget> &target, const Rectangle &rect, const Color color) {
    init();
    Renderer::set_target(target);
    rectangle_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    rectangle_material->set_uniform("pos", rect.position());
    rectangle_material->set_uniform("size", rect.size());
    rectangle_material->set_uniform("color", Vec4(color));
    Renderer::draw(quad, *rectangle_material, {});
}

void Graphics::draw(const std::shared_ptr<RenderTarget> &target, const Circle &circle, Color color) {
    init();
    Renderer::set_target(target);
    circle_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    circle_material->set_uniform("pos", circle.position());
    circle_material->set_uniform("radius", circle.radius);
    circle_material->set_uniform("color", Vec4(color));
    Renderer::draw(quad, *circle_material, {});
}

void Graphics::draw(const std::shared_ptr<RenderTarget> &target, const Rectangle &rect, const std::shared_ptr<Texture2D> &texture) {
    init();
    Renderer::set_target(target);
    texture_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    texture_material->set_uniform("pos", rect.position());
    texture_material->set_uniform("size", rect.size());
    texture_material->set_texture("tex", texture);
    Renderer::draw(quad, *texture_material, {});
}



void Graphics::draw(const std::shared_ptr<RenderTarget> &target, RectangleGroup &rects) {
    init();
    Renderer::set_target(target);
    rectangle_group_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    rects.build_mesh();
    Renderer::draw(rects.get_mesh(), *rectangle_group_material, {});
}


void Graphics::draw(const std::shared_ptr<RenderTarget> &target, CircleGroup &circles) {
    init();
    Renderer::set_target(target);
    circle_group_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    circles.build_mesh();
    Renderer::draw(circles.get_mesh(), *circle_group_material, {});
}

void Graphics::draw(const std::shared_ptr<RenderTarget> &target, const Text &text, const Vec2 &pos, const Color &color) {
    text.draw(target, pos, color);
}




void Graphics::draw_line(const std::shared_ptr<RenderTarget> &target, Line &line, const Color &color) {
    init();
    Renderer::set_target(target);
    glLineWidth(line.width);
    simple_line_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    simple_line_material->set_uniform("color", Vec4(color));
    Renderer::draw(simple_line, *simple_line_material, DrawOptions(GL_LINES));

}


void Graphics::init() {
    if (is_init) return;
    auto rectangle_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/rectangle_vert.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/shapes_frag.glsl")
        ->compile();
    rectangle_material = std::make_unique<Material>(rectangle_shader);
    auto circle_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/circle_vert.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/circle_frag.glsl")
        ->compile();
    circle_material = std::make_unique<Material>(circle_shader);
    auto texture_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/texture_vert.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/texture_frag.glsl")
        ->compile();
    texture_material = std::make_unique<Material>(texture_shader);
    auto rectangle_group_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/rectangle_group_vert.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/rectangle_group_frag.glsl")
        ->compile();
    rectangle_group_material = std::make_unique<Material>(rectangle_group_shader);
    auto circle_group_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/circle_group_vert.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/circle_group_frag.glsl")
        ->compile();
    circle_group_material = std::make_unique<Material>(circle_group_shader);
    auto line_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/line_vert.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/line_frag.glsl")
        ->compile();
    line_material = std::make_unique<Material>(line_shader);
    auto simple_line_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/simple_line_vert.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/simple_line_frag.glsl")
        ->compile();
    simple_line_material = std::make_unique<Material>(simple_line_shader);
    auto line_group_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/line_group_vert.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/line_group_frag.glsl")
        ->compile();
    line_group_material = std::make_unique<Material>(line_shader);
    auto simple_line_group_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/simple_line_group_vert.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/simple_line_group_frag.glsl")
        ->compile();
    simple_line_group_material = std::make_unique<Material>(simple_line_shader);
    quad = Mesh::create_quad(0, 0, 1, 1);
    simple_line = Mesh::create_line(0, 0, 1, 0);
    is_init = true;
}

