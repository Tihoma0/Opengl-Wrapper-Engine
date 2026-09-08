
#include "TextureAtlas.h"

#include "rendercore/core/math/ivec/IVec4.h"

TextureAtlas::TextureAtlas(const int width, const int height, const PixelStorageFormat fmt) : m_width(width), m_height(height) {
    texture = Texture2D::create();
    texture->allocate(width, height, fmt);
}


TextureAtlasArea TextureAtlas::add_texture(const Image &image, const IVec4 &uvs) {
    texture->update(image, uvs.x, uvs.y, uvs.z, uvs.w);
    return areas.size();
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
    texture->update(image, next_x, next_y, image.width(), image.height());
    const TextureAtlasArea area = areas.size();
    areas[area] = Vec4(next_x, next_y, next_x + image.width(), next_y + image.height());
    next_x += image.width();
    current_row_height = std::max(current_row_height, image.height());
    return area;
}

void TextureAtlas::bind() const {
    texture->bind();
}

void TextureAtlas::bind(const int slot) const {
    texture->bind(slot);
}

Vec4 TextureAtlas::get_uv(const TextureAtlasArea area) const {
    if (0 <= area && area < areas.size()) {
        return areas.at(area);
    }
    THROW_RUNTIME("Invalid texture atlas area");
}


