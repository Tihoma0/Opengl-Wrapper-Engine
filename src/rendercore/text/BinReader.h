#ifndef GLWRAPPER_II_BINREADER_H
#define GLWRAPPER_II_BINREADER_H
#include <cstdint>
#include <iosfwd>


namespace BinReader {
    uint16_t convert_short(uint16_t v);
    uint32_t convert_int(uint32_t v);
    float convert_fixed_point(uint32_t v);

    uint8_t read_byte(std::ifstream& stream);
    uint16_t read_short(std::ifstream& stream);
    uint32_t read_int(std::ifstream& stream);
};


#endif //GLWRAPPER_II_BINREADER_H
