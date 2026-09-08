#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H
#include <memory>
#include <unordered_map>

#include "Texture.h"
#include "glad/glad.h"
#include "rendercore/core/math/ivec/IVec4.h"
#include "rendercore/core/math/vec/Vec4.h"


using TextureAtlasArea = int;

class TextureAtlas {
public:
    TextureAtlas(int width, int height, PixelStorageFormat fmt = PixelStorageFormat::RGBA8);

    TextureAtlasArea add_texture(const Image &image, const IVec4 &uvs);
    TextureAtlasArea add_texture(const Image &image);

    void bind() const;
    void bind(int slot) const;

    [[nodiscard]] GLuint id() const { return texture->id(); }
    [[nodiscard]] int width() const { return m_width; }
    [[nodiscard]] int height() const { return m_height; }

    Vec4 get_uv(const TextureAtlasArea area) const;

private:
    std::shared_ptr<Texture2D> texture;
    std::unordered_map<TextureAtlasArea, Vec4> areas;
    int m_width;
    int m_height;
    int next_x = 0;
    int next_y = 0;
    int current_row_height = 0;
};



#endif //TEXTUREATLAS_H
