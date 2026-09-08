//
// Created by marku on 06.09.2026.
//

#ifndef FONT_H
#define FONT_H
#include <filesystem>
#include <string>
#include <vector>

#include " util/Path.h"
#include "freetype/freetype.h"
#include "rendercore/material/Material.h"
#include "rendercore/render/RenderTarget.h"
#include "rendercore/surface/textures/TextureAtlas.h"

struct Bitmap {
    std::vector<uint8_t> buf;
    int width;
    int height;
};
class Font {
public:
    explicit Font(const Path &path);

    [[nodiscard]] Bitmap get_bitmap(char ch, int size) const;

    void draw(const std::shared_ptr<RenderTarget> &target, const std::string &text, Vec2 pos, int size, Color color);

private:
    FT_Face face = nullptr;



    TextureAtlas atlas = TextureAtlas(100, 100);
    std::unordered_map<char32_t, TextureAtlasArea> glyph_map;

    static thread_local FT_Library lib;

    static thread_local std::unique_ptr<Material> material;
    static thread_local std::shared_ptr<Mesh> quad;

    inline static thread_local bool is_init = false;

    static void init();
};



#endif //FONT_H
