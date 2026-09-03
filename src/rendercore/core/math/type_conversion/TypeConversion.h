#ifndef TYPECONVERSION_H
#define TYPECONVERSION_H
#include <cstdint>


uint8_t f32_to_u8(float v);

uint16_t f32_to_f16(float const& f);

float f16_to_f32(uint16_t h);

float u8_to_f32(uint8_t v);

float u16_to_f32(uint16_t v);

float u32_to_f32(uint32_t v);
#endif //TYPECONVERSION_H
