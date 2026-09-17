#ifndef GLWRAPPER_II_BINREADER_H
#define GLWRAPPER_II_BINREADER_H
#include <cstdint>
#include <iosfwd>
#include <vector>


namespace BinReader {
    uint16_t convert_short(uint16_t v);
    uint32_t convert_int(uint32_t v);
    uint64_t convert_long(uint64_t);
    float convert_fixed_point(uint32_t v);

    uint8_t read_byte(std::ifstream& stream);
    uint16_t read_short(std::ifstream& stream);
    uint32_t read_int(std::ifstream& stream);
    uint64_t read_long(std::ifstream& stream);
    std::vector<uint16_t> read_short_array(std::ifstream& stream, uint32_t size);
};


#endif //GLWRAPPER_II_BINREADER_H
