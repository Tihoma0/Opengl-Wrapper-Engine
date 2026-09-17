#ifndef GLWRAPPER_II_TABLES_H
#define GLWRAPPER_II_TABLES_H
#include <cstdint>
#include <iosfwd>
#include <unordered_map>
#include <vector>


namespace Tables {
#pragma pack(push, 1)
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

    struct HeadTable {
        uint16_t majorVersion;
        uint16_t minorVersion;
        uint32_t fontRevision;
        uint32_t checkSumAdjustment;
        uint32_t magicNumber;
        uint16_t flags;
        uint16_t unitsPerEm;
        uint64_t created;
        uint64_t modified;
        int16_t  xMin;
        int16_t  yMin;
        int16_t  xMax;
        int16_t  yMax;
        uint16_t macStyle;
        uint16_t lowestRecPPEM;
        int16_t  fontDirectionHint;
        int16_t  indexToLocFormat;
        int16_t  glyphDataFormat;
    };

    struct MaxpTable {
        uint32_t version;
        uint16_t numGlyphs;
        uint16_t maxPoints;
        uint16_t maxContours;
        uint16_t maxCompositePoints;
        uint16_t maxCompositeContours;
        uint16_t maxZones;
        uint16_t maxTwilightPoints;
        uint16_t maxStorage;
        uint16_t maxFunctionDefs;
        uint16_t maxInstructionDefs;
        uint16_t maxStackElements;
        uint16_t maxSizeOfInstructions;
        uint16_t maxComponentElements;
        uint16_t maxComponentDepth;
    };

    struct CmapSubtableHeader {
        uint16_t platformID;
        uint16_t encodingID;
        uint32_t offset;
    };

    struct CmapHeader {
        uint16_t version;
        uint16_t numSubtables;
    };

#pragma pack(pop)

    FontHeader parse_header(std::ifstream& stream);

    TableInfo parse_table(std::ifstream& stream);

    std::unordered_map<std::string, TableInfo> parse_tables(std::ifstream& stream, int num_tables);

    HeadTable parse_head_table(std::ifstream& stream);

    MaxpTable parse_maxp_table(std::ifstream& stream);

    std::vector<uint32_t> parse_loca_table(std::ifstream& stream, uint32_t offset, uint16_t numGlyphs);

    void parse_cmap_header(std::ifstream& stream, uint32_t offset);

    std::ostream& operator<<(std::ostream& os, const FontHeader& header);
    std::ostream& operator<<(std::ostream& os, const TableInfo& table);
    std::ostream& operator<<(std::ostream& os, const HeadTable& table);
    std::ostream& operator<<(std::ostream& os, const MaxpTable& table);

};


#endif //GLWRAPPER_II_TABLES_H
