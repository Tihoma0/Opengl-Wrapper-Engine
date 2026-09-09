

#include "Font.h"

#include <cstring>
#include <iostream>

#include "rendercore/core/Warnings.h"
#include "rendercore/material/Shader.h"
#include "rendercore/mesh/Mesh.h"
#include "rendercore/render/Renderer.h"
#include "rendercore/surface/Color.h"
#include "rendercore/core/enums/mesh/DrawOptions.h"
#include "rendercore/surface/images/ImageProcessor.h"

thread_local FT_Library Font::lib;
thread_local std::shared_ptr<Mesh> Font::quad;
thread_local std::unique_ptr<Material> Font::material;

Font::Font(const Path &path) {
    init();
    const FT_Error error = FT_New_Face(
        lib,
        path.c_str(),
        0,
        &face
    );
    atlas = TextureAtlas::create(600, 600, PixelStorageFormat::R8);

    if (error) {
        std::cout << "Failed to load font: " << error << '\n';
        THROW_RUNTIME("Failed to load font");
    }
}

Bitmap Font::get_bitmap(const char ch, const int size) const {
    FT_Set_Pixel_Sizes(face, 0, size);
    if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
        std::cerr << "Failed to load glyph" << std::endl;
    const FT_Bitmap& bitmap = face->glyph->bitmap;
    std::vector<uint8_t> data(bitmap.rows * bitmap.width);
    std::memcpy(data.data(), bitmap.buffer, data.size());
    Bitmap b;
    b.buf = data;
    b.width = static_cast<int>(bitmap.width);
    b.height = static_cast<int>(bitmap.rows);
    return b;
}

void Font::draw(const std::shared_ptr<RenderTarget> &target, const std::string &text, Vec2 pos, const int size, const Color color) {
    target->bind();
    material->set_uniform("screen_size", Vec2(target->width(), target->height()));
    material->set_texture("tex", atlas);
    Vec2 current_pos = pos;
    for (const char ch : text) {
        if (!glyph_map.contains(ch)) {
            auto [buf, width, height] = get_bitmap(ch, size/2);
            auto img = Image(buf, width, height, PixelStorageFormat::R8);
            // auto img = Image("../data/Arm_back.png");
            // img = ImageProcessor::convert(img, PixelStorageFormat::R8);
            glyph_map[ch] = atlas->add_texture(img);
        }
        const auto area = glyph_map.at(ch);
        const auto uv = atlas->get_uv(area);
        material->set_uniform("uv", uv);
        material->set_uniform("color", Vec4(color));
        const float w = uv.z * atlas->width() * 2;
        const float h = uv.w * atlas->height() * 2;
        current_pos.y = pos.y - h + size;
        material->set_uniform("size", Vec2(w, h));
        material->set_uniform("pos", current_pos);
        current_pos.x += w;
        Renderer::draw(quad, *material, {});
    }
}

void Font::init() {
    if (!is_init) {
        if (FT_Init_FreeType(&lib)) {
            THROW_RUNTIME("Failed to initialize FreeType");
        }
        const auto shader = Shader::create()
            ->add_file_src(ShaderStage::Vertex, "../shaders/TextVertex.glsl")
            ->add_file_src(ShaderStage::Fragment, "../shaders/TextFragment.glsl")
            ->compile();
        material = std::make_unique<Material>(shader);
        quad = Mesh::create_quad(0, 0, 1, 1);
        is_init = true;
    }
}

