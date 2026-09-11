
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
    explicit Font(const Path &path, int default_character_size = 128);

    [[nodiscard]] Bitmap get_bitmap(char ch) const;

    void draw(const std::shared_ptr<RenderTarget> &target, const std::string &text, Vec2 pos, int size, Color color);

private:
    FT_Face face = nullptr;
    int default_character_size;
    std::shared_ptr<TextureAtlas> atlas;
    std::unordered_map<char32_t, TextureAtlasArea> glyph_map;

    static thread_local FT_Library lib;

    static thread_local std::unique_ptr<Material> material;
    static thread_local std::shared_ptr<Mesh> quad;

    inline static thread_local bool is_init = false;

    static void init();
};



#endif //FONT_H
