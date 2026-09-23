
#include "Graphics.h"

#include <iostream>

#include "core/math/shapes/BezierCurve.h"
#include "material/Material.h"
#include "material/Shader.h"
#include "mesh/Mesh.h"
#include "rendercore/core/enums/mesh/DrawOptions.h"
#include "core/math/shapes/Rectangle.h"
#include "core/math/shapes/Circle.h"
#include "core/math/shapes/CircleGroup.h"
#include "core/math/shapes/Line.h"
#include "core/math/shapes/LineGroup.h"
#include "core/math/shapes/RectangleGroup.h"
#include "opengl/GLTrack.h"
#include "render/Renderer.h"
#include "render/RenderTarget.h"
#include "text/Text.h"

thread_local std::unique_ptr<Material> rectangle_material;
thread_local std::unique_ptr<Material> circle_material;
thread_local std::unique_ptr<Material> line_material;
thread_local std::unique_ptr<Material> simple_line_material;
thread_local std::unique_ptr<Material> texture_material;
thread_local std::unique_ptr<Material> rectangle_group_material;
thread_local std::unique_ptr<Material> circle_group_material;
thread_local std::unique_ptr<Material> line_group_material;
thread_local std::unique_ptr<Material> simple_line_group_material;
thread_local std::unique_ptr<Material> simple_cubic_bezier_material;
thread_local std::unique_ptr<Material> simple_quadratic_bezier_material;
thread_local std::unique_ptr<Material> simple_cubic_bezier_group_material;
thread_local std::unique_ptr<Material> simple_quadratic_bezier_group_material;

thread_local bool is_init = false;
thread_local std::shared_ptr<Mesh> quad = nullptr;
thread_local std::shared_ptr<Mesh> line_quad = nullptr;
thread_local std::shared_ptr<Mesh> simple_line = nullptr;

void Graphics::draw(const std::shared_ptr<RenderTarget> &target, const Rectangle &rect, const Color &color) {
    init_rectangle();
    Renderer::set_target(target);
    rectangle_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    rectangle_material->set_uniform("pos", rect.position());
    rectangle_material->set_uniform("size", rect.size());
    rectangle_material->set_uniform("color", Vec4(color));
    Renderer::draw(quad, *rectangle_material, {});
}

void Graphics::draw(const std::shared_ptr<RenderTarget> &target, const Circle &circle, const Color &color) {
    init_circle();
    Renderer::set_target(target);
    circle_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    circle_material->set_uniform("pos", circle.position());
    circle_material->set_uniform("radius", circle.radius);
    circle_material->set_uniform("color", Vec4(color));
    Renderer::draw(quad, *circle_material, {});
}

void Graphics::draw(const std::shared_ptr<RenderTarget> &target, const Rectangle &rect, const std::shared_ptr<Texture2D> &texture) {
    init_texture();
    Renderer::set_target(target);
    texture_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    texture_material->set_uniform("pos", rect.position());
    texture_material->set_uniform("size", rect.size());
    texture_material->set_texture("tex", texture);
    Renderer::draw(quad, *texture_material, {});
}



void Graphics::draw(const std::shared_ptr<RenderTarget> &target, RectangleGroup &rects) {
    init_rectangle_group();
    Renderer::set_target(target);
    rectangle_group_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    rects.build_mesh();
    Renderer::draw(rects.get_mesh(), *rectangle_group_material, {});
}


void Graphics::draw(const std::shared_ptr<RenderTarget> &target, CircleGroup &circles) {
    init_circle_group();
    Renderer::set_target(target);
    circle_group_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    circles.build_mesh();
    Renderer::draw(circles.get_mesh(), *circle_group_material, {});
}

void Graphics::draw(const std::shared_ptr<RenderTarget> &target, const Text &text, const Vec2 &pos, const Color &color) {
    text.draw(target, pos, color);
}

void Graphics::draw(const std::shared_ptr<RenderTarget> &target, BezierCurve &curve, const Color &color, const int num_segments) {
    glLineWidth(curve.get_width());
    init_bezier();
    Renderer::set_target(target);

    const auto control_points = curve.get_control_points();
    const int num_control_points = static_cast<int>(control_points.size());
    set_patch_vertices(num_control_points);
    const auto mesh = curve.get_mesh();
    if (num_control_points == 4)
    {
        simple_cubic_bezier_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
        simple_cubic_bezier_material->set_uniform("color", Vec4(color));
        simple_cubic_bezier_material->set_uniform("num_segments", num_segments);
        simple_cubic_bezier_material->set_uniform("width", curve.get_width());
        Renderer::draw(mesh, *simple_cubic_bezier_material, DrawOptions(GL_PATCHES));
    }
    else {
        simple_quadratic_bezier_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
        simple_quadratic_bezier_material->set_uniform("color", Vec4(color));
        simple_quadratic_bezier_material->set_uniform("num_segments", num_segments);
        simple_quadratic_bezier_material->set_uniform("width", curve.get_width());
        Renderer::draw(mesh, *simple_quadratic_bezier_material, DrawOptions(GL_PATCHES));
    }
}

void Graphics::draw(const std::shared_ptr<RenderTarget> &target, BezierGroup &group, int num_segments) {
    glLineWidth(10);
    init_bezier_group();
    Renderer::set_target(target);
    group.build_cubic_mesh();
    group.build_quadratic_mesh();
    simple_cubic_bezier_group_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    simple_cubic_bezier_group_material->set_uniform("num_segments", num_segments);
    simple_quadratic_bezier_group_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    simple_quadratic_bezier_group_material->set_uniform("num_segments", num_segments);
    set_patch_vertices(4);
    Renderer::draw(group.get_cubic_mesh(), *simple_cubic_bezier_group_material, DrawOptions(GL_PATCHES, 0, 4));
    set_patch_vertices(3);
    Renderer::draw(group.get_quadratic_mesh(), *simple_quadratic_bezier_group_material, DrawOptions(GL_PATCHES, 0, 3));
}


void Graphics::draw(const std::shared_ptr<RenderTarget> &target, LineGroup &lines) {
    init_line_group();
    lines.build_quad_mesh();
    Renderer::set_target(target);
    line_group_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    Renderer::draw(lines.get_quad_mesh(), *line_group_material, {});
}

void Graphics::draw(const std::shared_ptr<RenderTarget> &target, const Line &line, const Color &color) {
    init_line();
    Renderer::set_target(target);
    line_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    line_material->set_uniform("start", line.start());
    const auto vec = line.along();
    line_material->set_uniform("normal", vec.normalize());
    line_material->set_uniform("length", vec.length());
    line_material->set_uniform("width", line.width);
    line_material->set_uniform("color", Vec4(color));
    Renderer::draw(line_quad, *line_material,{});
}


void Graphics::draw_line(const std::shared_ptr<RenderTarget> &target, const Line &line, const Color &color) {
    init_simple_line();
    Renderer::set_target(target);
    glLineWidth(line.width);
    simple_line_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    simple_line_material->set_uniform("start", line.start());
    simple_line_material->set_uniform("vector", line.along());
    simple_line_material->set_uniform("color", Vec4(color));
    Renderer::draw(simple_line, *simple_line_material, DrawOptions(GL_LINES));

}

void Graphics::draw_lines(const std::shared_ptr<RenderTarget> &target, LineGroup &lines, const float width) {
    init_simple_line_group();
    lines.build_line_mesh();
    glLineWidth(width);
    Renderer::set_target(target);
    simple_line_group_material->set_uniform("screen_size", Vec2(static_cast<float>(target->width()), static_cast<float>(target->height())));
    Renderer::draw(lines.get_line_mesh(), *simple_line_group_material, DrawOptions(GL_LINES));
}




std::unique_ptr<Material> create_material(
    const char* vertex_path,
    const char* fragment_path)
{
    std::cout << "creating material " << vertex_path << " " << fragment_path << std::endl;
    auto shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, vertex_path)
        ->add_file_src(ShaderStage::Fragment, fragment_path)
        ->compile();

    return std::make_unique<Material>(shader);
}

void Graphics::init_rectangle()
{
    if (rectangle_material)
        return;

    rectangle_material = create_material(
        "../shaders/rect/single/vert.glsl",
        "../shaders/rect/single/frag.glsl");

    if (!quad)
        quad = Mesh::create_quad(0, 0, 1, 1);
}

void Graphics::init_circle()
{
    if (circle_material)
        return;

    circle_material = create_material(
        "../shaders/circle/single/vert.glsl",
        "../shaders/circle/single/frag.glsl");
}

void Graphics::init_texture()
{
    if (texture_material)
        return;

    texture_material = create_material(
        "../shaders/texture/vert.glsl",
        "../shaders/texture/frag.glsl");
}

void Graphics::init_rectangle_group()
{
    if (rectangle_group_material)
        return;

    rectangle_group_material = create_material(
        "../shaders/rect/group/vert.glsl",
        "../shaders/rect/group/frag.glsl");
}

void Graphics::init_circle_group()
{
    if (circle_group_material)
        return;

    circle_group_material = create_material(
        "../shaders/circle/group/vert.glsl",
        "../shaders/circle/group/frag.glsl");
}

void Graphics::init_line()
{
    if (line_material)
        return;

    line_material = create_material(
        "../shaders/line/quad/single/vert.glsl",
        "../shaders/line/quad/single/frag.glsl");

    if (!line_quad)
        line_quad = Mesh::create_quad(-0.5, -0.5, 1, 1);
}

void Graphics::init_simple_line()
{
    if (simple_line_material)
        return;

    glEnable(GL_LINE_SMOOTH);

    simple_line_material = create_material(
        "../shaders/line/simple/single/vert.glsl",
        "../shaders/line/simple/single/frag.glsl");

    if (!simple_line)
        simple_line = Mesh::create_line(0, 0, 1, 1);
}

void Graphics::init_line_group()
{
    if (line_group_material)
        return;

    line_group_material = create_material(
        "../shaders/line/quad/group/vert.glsl",
        "../shaders/line/quad/group/frag.glsl");
}

void Graphics::init_simple_line_group()
{
    if (simple_line_group_material)
        return;

    glEnable(GL_LINE_SMOOTH);

    simple_line_group_material = create_material(
        "../shaders/line/simple/group/vert.glsl",
        "../shaders/line/simple/group/frag.glsl");
}

void Graphics::init_bezier()
{
    if (simple_cubic_bezier_material && simple_quadratic_bezier_material)
        return;
    auto cubic_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/bezier/cubic/simple/single/vert.glsl")
        ->add_file_src(ShaderStage::TessControl, "../shaders/bezier/cubic/simple/single/tess_control.glsl")
        ->add_file_src(ShaderStage::TessEval, "../shaders/bezier/cubic/simple/single/tess_eval.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/bezier/cubic/simple/single/frag.glsl")
        ->compile();
    simple_cubic_bezier_material = std::make_unique<Material>(cubic_shader);
    auto quadratic_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/bezier/quadratic/simple/single/vert.glsl")
        ->add_file_src(ShaderStage::TessControl, "../shaders/bezier/quadratic/simple/single/tess_control.glsl")
        ->add_file_src(ShaderStage::TessEval, "../shaders/bezier/quadratic/simple/single/tess_eval.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/bezier/quadratic/simple/single/frag.glsl")
        ->compile();
    simple_quadratic_bezier_material = std::make_unique<Material>(quadratic_shader);
}

void Graphics::init_bezier_group() {
    if (simple_cubic_bezier_group_material && simple_quadratic_bezier_group_material)
        return;
    auto cubic_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/bezier/cubic/simple/group/vert.glsl")
        ->add_file_src(ShaderStage::TessControl, "../shaders/bezier/cubic/simple/group/tess_control.glsl")
        ->add_file_src(ShaderStage::TessEval, "../shaders/bezier/cubic/simple/group/tess_eval.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/bezier/cubic/simple/group/frag.glsl")
        ->compile();
    simple_cubic_bezier_group_material = std::make_unique<Material>(cubic_shader);
    auto quadratic_shader = Shader::create()
        ->add_file_src(ShaderStage::Vertex, "../shaders/bezier/quadratic/simple/group/vert.glsl")
        ->add_file_src(ShaderStage::TessControl, "../shaders/bezier/quadratic/simple/group/tess_control.glsl")
        ->add_file_src(ShaderStage::TessEval, "../shaders/bezier/quadratic/simple/group/tess_eval.glsl")
        ->add_file_src(ShaderStage::Fragment, "../shaders/bezier/quadratic/simple/group/frag.glsl")
        ->compile();
    simple_quadratic_bezier_group_material = std::make_unique<Material>(quadratic_shader);
}


void Graphics::init() {
    init_line();
    init_line_group();
    init_simple_line();
    init_simple_line_group();
    init_rectangle();
    init_rectangle_group();
    init_circle();
    init_circle_group();
    init_texture();
    init_bezier();
}

