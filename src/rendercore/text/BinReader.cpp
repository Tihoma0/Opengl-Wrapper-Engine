#include "BinReader.h"

#include <fstream>

uint16_t BinReader::convert_short(const uint16_t v) {
    return ((v << 8) & 0xff00) | ((v >> 8) & 0xff);
}

uint32_t BinReader::convert_int(const uint32_t v) {
    return ((v << 24) & 0xff000000) | ((v << 8) & 0xff0000) | ((v >> 8) & 0xff00) | ((v >> 24) & 0xff);
}

uint64_t BinReader::convert_long(const uint64_t v) {
    return ((v << 56) & 0xff00000000000000L) |
           ((v << 40) & 0xff000000000000L) |
           ((v << 24) & 0xff0000000000L) |
           ((v << 8)  & 0xff00000000L) |
           ((v >> 8)  & 0xff000000L) |
           ((v >> 24) & 0xff0000L) |
           ((v >> 40) & 0xff00L) |
           ((v >> 56) & 0xffL);
}


float BinReader::convert_fixed_point(const uint32_t v) {
    return static_cast<float>(v) / 65536.0f;
}

uint8_t BinReader::read_byte(std::ifstream& stream) {
    uint8_t out = 0;
    stream.read(reinterpret_cast<char*>(&out), 1);
    return out;
}

uint16_t BinReader::read_short(std::ifstream& stream) {
    uint16_t out = 0;
    stream.read(reinterpret_cast<char*>(&out), 2);
    return convert_short(out);
}

uint32_t BinReader::read_int(std::ifstream& stream) {
    uint32_t out = 0;
    stream.read(reinterpret_cast<char*>(&out), 4);
    return convert_int(out);
}

uint64_t BinReader::read_long(std::ifstream &stream) {
    uint64_t out = 0;
    stream.read(reinterpret_cast<char*>(&out), 8);
    return convert_long(out);
}
