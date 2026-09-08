#ifndef IMAGE_H
#define IMAGE_H

#include <vector>

#include " util/Path.h"
#include "rendercore/core/enums/textures/PixelStorageFormat.h"


class Color;

enum ImageType {
    ColorImage,
    DepthImage,
    StencilImage
};

class Image {

public:

    Image(const Path &path, bool forceRGBA = false);

    Image(const std::vector<unsigned char> &data, int w, int h, PixelStorageFormat format);

    Image(Color color, int w, int h, PixelStorageFormat fmt);

    Image(int w, int h, PixelStorageFormat fmt);

    Image(int w, int h, PixelStorageFormat fmt, int pixel_size);

    void set_type(const ImageType type) { m_type = type; }
    [[nodiscard]] int width() const { return m_width; }
    [[nodiscard]] int height() const { return m_height; }
    [[nodiscard]] ImageType type() const { return m_type; }
    [[nodiscard]] PixelStorageFormat format() const { return m_format; }
    [[nodiscard]] std::vector<uint8_t>& buffer() { return m_buffer; }
    [[nodiscard]] const std::vector<uint8_t>& buffer() const { return m_buffer; }
    [[nodiscard]] Path path() const { return m_path; }

    void save_png(const Path &path) const;


private:
    Path m_path;
    PixelStorageFormat m_format = PixelStorageFormat::RGB8;
    std::vector<uint8_t> m_buffer = {};
    int m_width = 0;
    int m_height = 0;
    ImageType m_type = ColorImage;

    void deserialize(bool forceRGBA);
};
#endif //IMAGE_H
