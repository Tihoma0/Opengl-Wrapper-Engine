

#include "Font.h"

#include <cstring>
#include <iostream>

#include "rendercore/core/Warnings.h"
#include "rendercore/material/Shader.h"
#include "rendercore/mesh/Mesh.h"
#include "rendercore/surface/Color.h"

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

void Font::draw(const std::shared_ptr<RenderTarget> &target, const std::string &text, Vec2 pos, const int size, Color color) {
    target->bind();
    atlas.bind(); // TODO: change after adding atlas support for the rest of the engine
    for (const char ch : text) {
        if (!glyph_map.contains(ch)) {
            auto [buf, width, height] = get_bitmap(ch, size);
            auto img = Image(buf, width, height, PixelStorageFormat::RGBA8);
            glyph_map[ch] = atlas.add_texture(img);
        }
        const auto area = glyph_map.at(ch);
        const auto uv = atlas.get_uv(area);
        material->set_uniform("uv", uv);
        material->set_uniform("color", Vec4(color));
        material->set_uniform("pos", pos);
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

