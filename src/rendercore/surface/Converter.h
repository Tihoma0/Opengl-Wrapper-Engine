//
// Created by marku on 15.05.2026.
//

#ifndef CONVERTER_H
#define CONVERTER_H
#include "Image.h"
#include "../target/Renderbuffer.h"
#include "textures/Texture.h"

class Converter {
public:
    static Image toImage(Renderbuffer rbuf) {
        int pixel_bytes = get_bytes_per_pixel(rbuf.format());
        std::vector<uint8_t> buffer(pixel_bytes * rbuf.width() * rbuf.height());
        rbuf.bind();
        glReadPixels(0, 0, rbuf.width(), rbuf.height(), GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
        return Image(buffer, rbuf.width(), rbuf.height(), rbuf.format());
    }

    static Image toImage(Texture2D tex) {
        int pixel_bytes = get_bytes_per_pixel(tex.format());
        std::vector<uint8_t> buffer(pixel_bytes * tex.width() * tex.height());
        tex.bind();
        glReadPixels(0, 0, tex.width(), tex.height(), GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
        return Image(buffer, tex.width(), tex.height(), tex.format());
    }
};
#endif //CONVERTER_H
