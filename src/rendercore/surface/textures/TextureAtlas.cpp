
#include "TextureAtlas.h"

#include "rendercore/core/math/ivec/IVec4.h"

TextureAtlas::TextureAtlas(const int width, const int height, const PixelStorageFormat fmt) {
    allocate(width, height, fmt);
}


std::shared_ptr<TextureAtlas> TextureAtlas::create(const int width, const int height, const PixelStorageFormat fmt) {
    return std::shared_ptr<TextureAtlas>(new TextureAtlas(width, height, fmt));
}

TextureAtlasArea TextureAtlas::add_texture(const Image &image, const IVec4 &uvs) {
    update(image, uvs.x, uvs.y, uvs.z, uvs.w);
    return static_cast<TextureAtlasArea>(areas.size());
}

TextureAtlasArea TextureAtlas::add_texture(const Image &image) {
    if (next_x + image.width() > m_width) {
        next_x = 0;
        next_y += current_row_height;
        current_row_height = 0;
    }
    if (next_y + image.height() > m_height) {
        throw std::runtime_error("Texture atlas is full");
    }
    update(image, next_x, next_y, image.width(), image.height());
    const auto area = static_cast<TextureAtlasArea>(areas.size());
    areas[area] = Vec4(
        static_cast<float>(next_x) / static_cast<float>(width()),
        static_cast<float>(next_y) / static_cast<float>(height()),
        static_cast<float>(image.width()) / static_cast<float>(width()),
        static_cast<float>(image.height()) / static_cast<float>(height())
    );
    next_x += image.width();
    current_row_height = std::max(current_row_height, image.height());
    return area;
}

Vec4 TextureAtlas::get_uv(const TextureAtlasArea area) const {
    if (0 <= area && area < areas.size()) {
        return areas.at(area);
    }
    THROW_RUNTIME("Invalid texture atlas area");
}


