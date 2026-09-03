#ifndef CONVERTER_H
#define CONVERTER_H
#include "images/Image.h"
#include "rendercore/render/Renderbuffer.h"
#include "textures/Texture.h"

class Converter {
public:
    static Image toImage(const Renderbuffer &rbuf) {
        const int pixel_bytes = get_bytes_per_pixel(rbuf.format());
        std::vector<uint8_t> buffer(pixel_bytes * rbuf.width() * rbuf.height());
        rbuf.bind();
        glReadPixels(0, 0, rbuf.width(), rbuf.height(), GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
        return Image(buffer, rbuf.width(), rbuf.height(), rbuf.format());
    }

    static Image toImage(const Texture2D &tex) {
        const int pixel_bytes = get_bytes_per_pixel(tex.format());
        std::vector<uint8_t> buffer(pixel_bytes * tex.width() * tex.height());
        tex.bind();
        glReadPixels(0, 0, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
        return Image(buffer, tex.width(), tex.height(), tex.format());
    }
};
#endif //CONVERTER_H
