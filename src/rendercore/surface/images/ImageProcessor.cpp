
#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <iostream>

#include "Image.h"
#include "rendercore/core/math/type_conversion/TypeConversion.h"
#include "rendercore/surface/Color.h"


class ImageProcessor {
public:
    static void flip(Image &image) {
        int channels = get_channels(image.format());
        int rowSize = image.width() * channels;
        for (int y = 0; y < image.height() / 2; ++y) {
            uint8_t* top = image.buffer().data() + y * rowSize;
            uint8_t* bottom = image.buffer().data() + (image.height() - 1 - y) * rowSize;
            std::swap_ranges(top, top + rowSize, bottom);
        }
    }

    static Image convert(const Image& src, const PixelStorageFormat format) {
        std::cout << "Converting " << to_string(src.format()) << " to " << to_string(format) << std::endl;
        auto dest = Image(src.width(), src.height(), format);
        const int bytes_src = get_bytes_per_pixel(src.format());
        const int bytes_dest = get_bytes_per_pixel(dest.format());
        const std::vector<uint8_t>& buf_src = src.buffer();
        std::vector<uint8_t>& buf_dest = dest.buffer();
        const int pixels = buf_src.size() / bytes_src;
        const PixelStorageFormat src_format = src.format();
        for (int i = 0; i < pixels; ++i) {
            Color src_color = Color::from_buffer(const_cast<uint8_t *>(buf_src.data()) + i * bytes_src, src_format);
            src_color.pack(format, buf_dest.data() + i * bytes_dest);
        }
        return dest;
    }

private:
    static void convert_channels(Image& src, Image& dest) {
        const int channels_src = get_channels(src.format());
        const int channels_dest = get_channels(dest.format());
        const int channels_common = std::min(channels_src, channels_dest);
        const std::vector<uint8_t>& buf_src = src.buffer();
        std::vector<uint8_t>& buf_dest = dest.buffer();
        const int pixels = buf_src.size() / channels_src;
        for (int i = 0; i < pixels; ++i) {
            for (int j = 0; j < channels_common; ++j) {
                buf_dest[i * channels_dest + j] = buf_src[i * channels_src + j];
            }
            for (int j = channels_common; j < channels_dest; ++j) {
                buf_dest[i * channels_dest + j] = 0;
            }
        }
    }
};
#endif //IMAGEPROCESSOR_H
