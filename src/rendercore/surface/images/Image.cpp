
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Image.h"

#include <cstring>
#include <stb_image.h>
#include <stb_image_write.h>
#include <iostream>
#include <string>
#include <vector>

#include "rendercore/surface/Color.h"
#include "rendercore/core/enums/textures/PixelStorageFormat.h"




Image::Image(const std::string &path, const bool forceRGBA) {
    m_path = path;
    deserialize(forceRGBA);
}

Image::Image(const std::vector<unsigned char> &data, const int w, const int h, const PixelStorageFormat format) {
    m_width = w;
    m_height = h;
    m_format = format;
    m_buffer = data;
}

Image::Image(const Color color, const int w, const int h, const PixelStorageFormat fmt) {
    m_width = w;
    m_height = h;
    m_format = fmt;

    size_t bytes_per_pixel = get_bytes_per_pixel(fmt);
    m_buffer.resize(w * h * bytes_per_pixel);

    const std::vector<uint8_t> colors_packed = color.pack(fmt);
    if (bytes_per_pixel == 4) {
        uint32_t scalar_color;
        std::memcpy(&scalar_color, colors_packed.data(), 4);
        const auto typed_ptr = reinterpret_cast<uint32_t*>(m_buffer.data());
        const size_t total_pixels = w * h;
        std::fill_n(typed_ptr, total_pixels, scalar_color);
    }
    else {
        std::memcpy(m_buffer.data(), colors_packed.data(), bytes_per_pixel);
        size_t filled = bytes_per_pixel;
        while (filled < m_buffer.size()) {
            const size_t chunk = std::min(filled, m_buffer.size() - filled);
            std::memcpy(m_buffer.data() + filled, m_buffer.data(), chunk);
            filled += chunk;
        }
    }
}

Image::Image(const int w, const int h, const PixelStorageFormat fmt) {
    m_width = w;
    m_height = h;
    m_format = fmt;
    m_buffer.resize(w * h * get_bytes_per_pixel(fmt));
}

Image::Image(const int w, const int h, const PixelStorageFormat fmt, const int pixel_size) {
    m_width = w;
    m_height = h;
    m_format = fmt;
    m_buffer.resize(w * h * pixel_size);
}

void Image::save_png(const std::string& path) const {
    std::vector<uint8_t> flipped(m_buffer.size());
    int rowSize = m_width * get_bytes_per_pixel(m_format);
    for (int y = 0; y < m_height; ++y)
    {
        const uint8_t* src =
            m_buffer.data() + (m_height - 1 - y) * rowSize;
        uint8_t* dst =
            flipped.data() + y * rowSize;
        std::memcpy(dst, src, rowSize);
    }
    stbi_write_png(path.c_str(), m_width, m_height, get_bytes_per_pixel(m_format), flipped.data(), m_width * get_bytes_per_pixel(m_format));
}


void Image::deserialize(const bool forceRGBA) {
    int in_channels;
    unsigned char *data = stbi_load(m_path.c_str(), &m_width, &m_height, &in_channels, forceRGBA ? 4 : 0);
    if (!data) {
        std::cerr << "Failed to load image: " << m_path << std::endl;
        return;
    }
    const int out_channels = forceRGBA ? 4 : in_channels;
    m_format = out_channels == 3 ? PixelStorageFormat::RGB8 : PixelStorageFormat::RGBA8;
    m_buffer.resize(m_width * m_height * out_channels);
    const int rowSize = m_width * out_channels;
    for (int y = 0; y < m_height / 2; ++y) {
        uint8_t* top = data + y * rowSize;
        uint8_t* bottom = data + (m_height - 1 - y) * rowSize;
        std::swap_ranges(top, top + rowSize, bottom);
    }
    m_buffer.assign(data, data + m_width * m_height * out_channels);
    stbi_image_free(data);
}
