#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H
#include <memory>
#include <unordered_map>

#include "Texture.h"
#include "rendercore/core/math/ivec/IVec4.h"
#include "rendercore/core/math/vec/Vec4.h"


using TextureAtlasArea = int;

class TextureAtlas : public Texture2D, public std::enable_shared_from_this<TextureAtlas> {
public:

    TextureAtlas(const TextureAtlas&) = delete;
    TextureAtlas& operator=(const TextureAtlas&) = delete;

    TextureAtlas(TextureAtlas&&) noexcept = default;
    TextureAtlas& operator=(TextureAtlas&&) noexcept = default;


    static std::shared_ptr<TextureAtlas> create(int width, int height, PixelStorageFormat fmt = PixelStorageFormat::RGBA8);
    TextureAtlasArea add_texture(const Image &image, const IVec4 &uvs);
    TextureAtlasArea add_texture(const Image &image);


    Vec4 get_uv(TextureAtlasArea area) const;

private:
    TextureAtlas(int width, int height, PixelStorageFormat fmt);
    std::unordered_map<TextureAtlasArea, Vec4> areas;
    int next_x = 0;
    int next_y = 0;
    int current_row_height = 0;
};



#endif //TEXTUREATLAS_H
