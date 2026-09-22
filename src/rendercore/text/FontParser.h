#ifndef GLWRAPPER_II_TABLES_H
#define GLWRAPPER_II_TABLES_H
#include <cstdint>
#include <iosfwd>
#include <unordered_map>
#include <vector>


class Line;
class QuadraticBezierCurve;

namespace FontParser {
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

    struct CmapFormat4Header {
        uint16_t format;
        uint16_t length;
        uint16_t language;
        uint16_t segCountX2;
        uint16_t searchRange;
        uint16_t entrySelector;
        uint16_t rangeShift;
    };

    struct GlyphHeader {
        int16_t numberOfContours;
        int16_t xMin;
        int16_t yMin;
        int16_t xMax;
        int16_t yMax;
    };

#pragma pack(pop)

    struct CmapFormat4Data {
        std::vector<uint16_t> endCodes;
        std::vector<uint16_t> startCodes;
        std::vector<uint16_t> idDeltas;
        std::vector<uint16_t> idRangeOffsets;
        int glyphIdArrayOffset;
        int rangeOffsetsPos;
        int num_segments;
    };

    struct GlyphPoint {
        int16_t x;
        int16_t y;
        bool on_curve;
    };

    struct GlyphData {
        std::vector<GlyphPoint> points;
        std::vector<uint16_t> end_points;
    };

    struct GlyphOutline {
        std::vector<Line> lines;
        std::vector<QuadraticBezierCurve> curves;
    };

    FontHeader parse_header(std::ifstream& stream);

    // TableInfo parse_table(std::ifstream& stream);

    std::unordered_map<std::string, TableInfo> parse_tables(std::ifstream& stream, int num_tables);

    HeadTable parse_head_table(std::ifstream& stream);

    MaxpTable parse_maxp_table(std::ifstream& stream);

    std::vector<uint32_t> parse_loca_table(std::ifstream& stream, uint32_t offset, uint16_t numGlyphs);

    CmapHeader parse_cmap_header(std::ifstream& stream, uint32_t offset);

    // CmapSubtableHeader parse_cmap_subtable_header(std::ifstream& stream);

    uint32_t find_unicode_subtable_offset(std::ifstream &stream, uint32_t cmap_start_offset, uint16_t numSubtables);

    // CmapFormat4Header parse_cmap_format4_header(std::ifstream &stream);

    CmapFormat4Data read_cmap_format4_data(std::ifstream& stream);

    uint16_t get_glyph_id(std::ifstream& stream, const CmapFormat4Data& map_data, char32_t code);

    GlyphHeader parse_glyph_header(std::ifstream& stream);

    GlyphData get_glyph_points(std::ifstream& stream, const GlyphHeader& header);

    GlyphOutline points_to_outline(FontParser::GlyphData glyph_data);


    std::ostream& operator<<(std::ostream& os, const FontHeader& header);
    std::ostream& operator<<(std::ostream& os, const TableInfo& table);
    std::ostream& operator<<(std::ostream& os, const HeadTable& table);
    std::ostream& operator<<(std::ostream& os, const MaxpTable& table);
    std::ostream& operator<<(std::ostream& os, const CmapFormat4Header& header);
    std::ostream& operator<<(std::ostream& os, const GlyphHeader& table);

};


#endif //GLWRAPPER_II_TABLES_H
