
#include "Graphics.h"

#include "material/Material.h"
#include "material/Shader.h"
#include "mesh/Mesh.h"
#include "rendercore/core/enums/mesh/DrawOptions.h"
#include "core/math/Rectangle.h"
#include "core/math/Circle.h"
#include "render/Renderer.h"

thread_local std::unique_ptr<Material> rectangle_material;
thread_local std::unique_ptr<Material> circle_material;
thread_local std::unique_ptr<Material> texture_material;
thread_local std::unique_ptr<Material> rectangle_group_material;
thread_local std::unique_ptr<Material> circle_group_material;
thread_local bool is_init = false;
std::shared_ptr<Mesh> quad;

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

void Graphics::draw(const std::shared_ptr<RenderTarget> &target, const Text &text, Vec2 pos) {
    init();
    Renderer::set_target(target);
    auto texture = Texture2D::create();
    for (const auto &character : text.text) {
        auto bitmap = text.font.get_bitmap(character, text.size);
        auto img = Image(bitmap.buf, bitmap.width, bitmap.height, PixelStorageFormat::R8);
        texture->set_image(img);
        Rectangle rect = {pos, Vec2(bitmap.width, bitmap.height)};
        draw(target, rect, texture);
        pos.x += bitmap.width;
    }
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
    quad = Mesh::create_quad(0, 0, 1, 1);
    is_init = true;
}

