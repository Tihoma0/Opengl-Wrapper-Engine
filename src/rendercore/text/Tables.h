#ifndef GLWRAPPER_II_TABLES_H
#define GLWRAPPER_II_TABLES_H
#include <cstdint>


namespace Tables {
    struct FontHeader {
        uint32_t fontVersion;
        uint16_t numTables;
        uint16_t searchRange;
        uint16_t entrySelector;
        uint16_t rangeShift;
    };

    struct TableInfo {
        char title[4];
        uint32_t checkSum;
        uint32_t offset;
        uint32_t length;
    };
};


#endif //GLWRAPPER_II_TABLES_H
