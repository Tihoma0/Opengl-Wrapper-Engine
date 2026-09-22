

#include "Font.h"

#include <cstring>
#include <iostream>

#include "rendercore/core/Warnings.h"
#include "rendercore/material/Shader.h"
#include "rendercore/mesh/Mesh.h"
#include "rendercore/render/Renderer.h"
#include "rendercore/surface/Color.h"
#include "rendercore/core/enums/mesh/DrawOptions.h"
#include "rendercore/core/math/shapes/BezierCurve.h"
#include "rendercore/core/math/shapes/Line.h"
#include "rendercore/surface/textures/Sampler.h"

thread_local FT_Library BitmapFont::lib;
thread_local std::shared_ptr<Mesh> BitmapFont::quad;
thread_local std::unique_ptr<Material> BitmapFont::material;

Font::Font(const Path &path) {
    font_file = std::ifstream(path.filesystem_path(), std::ios::binary);
    if (!font_file) {
        THROW_RUNTIME("Error when opening " + path.string());
    }
    using namespace FontParser;
    font_file.seekg(0, std::ios::beg);
    header = parse_header(font_file);
    tables = parse_tables(font_file, header.numTables);
    head_table = parse_head_table(font_file);
    font_file.seekg(tables["maxp"].offset, std::ios::beg);
    maxp_table = parse_maxp_table(font_file);
    loca = parse_loca_table(font_file, tables["loca"].offset, maxp_table.numGlyphs);
    cmap_header = parse_cmap_header(font_file, tables["cmap"].offset);
    format4_offset = find_unicode_subtable_offset(font_file, tables["cmap"].offset, cmap_header.numSubtables);
    if (format4_offset == 0) {
        THROW_RUNTIME("No unicode subtable found");
    }
    font_file.seekg(format4_offset, std::ios::beg);
    cmap_format4_glyph_ids = read_cmap_format4_data(font_file);
}

FontParser::GlyphOutline Font::get_outline(const char32_t glyph) {
    if (outlines.contains(glyph))
        return outlines[glyph];
    using namespace FontParser;
    const auto glyph_pos = get_glyph_id(font_file, cmap_format4_glyph_ids, glyph);
    const auto start_pos = loca[glyph_pos];
    const auto end_pos = loca[glyph_pos + 1];
    font_file.seekg(start_pos + tables["glyf"].offset, std::ios::beg);
    const auto glyph_header = parse_glyph_header(font_file);
    if (glyph_header.numberOfContours <= 0)
        return {};
    const auto glyph_data = get_glyph_points(font_file, glyph_header);
    auto outline = points_to_outline(glyph_data);
    outlines[glyph] = outline;
    return outline;
}

BitmapFont::BitmapFont(const Path &path, int default_character_size) {
    init();
    const FT_Error error = FT_New_Face(
        lib,
        path.c_str(),
        0,
        &face
    );
    atlas = TextureAtlas::create(default_character_size * 6, default_character_size * 6, PixelStorageFormat::R8);
    atlas->set_default_sampler(Sampler::create()->set_filters(Filter::LinearMipmapLinear, Filter::LinearMipmapLinear));
    this->default_character_size = default_character_size;
    if (error) {
        std::cout << "Failed to load font: " << error << '\n';
        THROW_RUNTIME("Failed to load font");
    }
}

Bitmap BitmapFont::get_bitmap(const char ch) const {
    FT_Set_Pixel_Sizes(face, 0, default_character_size);
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

void BitmapFont::draw(const std::shared_ptr<RenderTarget> &target, const std::string &text, const Vec2 pos, const int size, const Color color) {
    target->bind();
    material->set_uniform("screen_size", Vec2(target->width(), target->height()));
    material->set_texture("tex", atlas);
    Vec2 current_pos = pos;
    for (const char ch : text) {
        if (!glyph_map.contains(ch)) {
            auto [buf, width, height] = get_bitmap(ch);
            auto img = Image(buf, width, height, PixelStorageFormat::R8);
            glyph_map[ch] = atlas->add_texture(img);
        }
        const auto area = glyph_map.at(ch);
        const auto uv = atlas->get_uv(area);
        material->set_uniform("uv", uv);
        material->set_uniform("color", Vec4(color));
        const float w = uv.z * static_cast<float>(atlas->width()) * static_cast<float>(size) / static_cast<float>(default_character_size);
        const float h = uv.w * static_cast<float>(atlas->height()) * static_cast<float>(size) / static_cast<float>(default_character_size);
        current_pos.y = pos.y - h + size;
        material->set_uniform("size", Vec2(w, h));
        material->set_uniform("pos", current_pos);
        current_pos.x += w;
        Renderer::draw(quad, *material, {});
    }
}

void BitmapFont::init() {
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

