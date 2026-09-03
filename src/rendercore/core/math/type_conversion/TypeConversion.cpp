#include "TypeConversion.h"

#include <algorithm>
#include <cstdint>


uint8_t f32_to_u8(float v) {
    v = std::clamp(v, 0.0f, 1.0f);
    return static_cast<uint8_t>(v * 255.0f + 0.5f);
}

uint16_t f32_to_f16(float const& f) {
    const auto bytes = std::bit_cast<uint32_t>(f);
    const int sign =  (bytes >> 16) & 0x00008000;
    int exponent = ((bytes >> 23) & 0x000000ff) - (127 - 15);
    int val =   bytes        & 0x007fffff;
    if(exponent <= 0)
    {
        if(exponent < -10)
        {
            return static_cast<uint16_t>(sign);
        }

        val = (val | 0x00800000) >> (1 - exponent);

        if(val & 0x00001000)
            val += 0x00002000;
        return static_cast<uint16_t>(sign | (val >> 13));
    }
    if(exponent == 0xff - (127 - 15)) {
        if(val == 0)
            return static_cast<uint16_t>(sign | 0x7c00);
        val >>= 13;
        return static_cast<uint16_t>(sign | 0x7c00 | val | (val == 0));
    }
    if(val &  0x00001000) {
        val += 0x00002000;
        if(val & 0x00800000)
        {
            val =  0;
            exponent += 1;
        }
    }
    if (exponent > 30)
    {
        exponent = 31;
        val = 0;
    }
    return static_cast<uint16_t>(sign | (exponent << 10) | (val >> 13));
}

float f16_to_f32(const uint16_t h)
{
    const uint32_t sign     = (h & 0x8000) << 16;
    uint32_t exponent = (h >> 10) & 0x1f;
    uint32_t mantissa = h & 0x3ff;

    uint32_t bits;

    if (exponent == 0)
    {
        if (mantissa == 0)
        {
            bits = sign;
        }
        else
        {
            exponent = 1;

            while ((mantissa & 0x400) == 0)
            {
                mantissa <<= 1;
                exponent--;
            }

            mantissa &= 0x3ff;

            exponent = exponent + (127 - 15);

            bits =
                sign |
                (exponent << 23) |
                (mantissa << 13);
        }
    }
    else if (exponent == 31)
    {
        bits =
            sign |
            0x7f800000 |
            (mantissa << 13);
    }
    else
    {
        exponent = exponent + (127 - 15);

        bits =
            sign |
            (exponent << 23) |
            (mantissa << 13);
    }

    return std::bit_cast<float>(bits);
}

float u8_to_f32(uint8_t v) {
    return static_cast<float>(v) / 255.0f;
}

float u16_to_f32(uint16_t v) {
    return static_cast<float>(v) / 65535.0f;
}

float u32_to_f32(uint32_t v) {
    return static_cast<float>(v) / 4294967295.0f;
}

