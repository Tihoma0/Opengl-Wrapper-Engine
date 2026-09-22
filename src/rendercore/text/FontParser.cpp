
#include "FontParser.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iosfwd>
#include <iostream>

#include "BinReader.h"
#include "rendercore/core/Warnings.h"
#include "rendercore/core/math/shapes/BezierCurve.h"
#include "rendercore/core/math/shapes/Line.h"

FontParser::FontHeader FontParser::parse_header(std::ifstream& stream) {
    FontHeader result{};
    stream.read(reinterpret_cast<char*>(&result), sizeof(FontHeader));
    result.fontVersion = BinReader::convert_int(result.fontVersion);
    result.numTables = BinReader::convert_short(result.numTables);
    result.searchRange = BinReader::convert_short(result.searchRange);
    result.entrySelector = BinReader::convert_short(result.entrySelector);
    result.rangeShift = BinReader::convert_short(result.rangeShift);
    return result;
}

FontParser::TableInfo parse_table(std::ifstream &stream) {
    FontParser::TableInfo result{};
    stream.read(reinterpret_cast<char*>(&result), sizeof(FontParser::TableInfo));
    result.offset = BinReader::convert_int(result.offset);
    result.length = BinReader::convert_int(result.length);
    return result;
}


std::unordered_map<std::string, FontParser::TableInfo> FontParser::parse_tables(std::ifstream &stream, const int num_tables) {
    std::unordered_map<std::string, TableInfo> result;
    for (int i = 0; i < num_tables; ++i) {
        auto table = parse_table(stream);
        std::string key(table.title, 4);
        result[key] = table;
    }
    return result;
}

FontParser::HeadTable FontParser::parse_head_table(std::ifstream &stream) {
    HeadTable result{};
    stream.read(reinterpret_cast<char*>(&result), sizeof(HeadTable));
    result.majorVersion       = BinReader::convert_short(result.majorVersion);
    result.minorVersion       = BinReader::convert_short(result.minorVersion);
    result.fontRevision       = BinReader::convert_int(result.fontRevision);
    result.checkSumAdjustment = BinReader::convert_int(result.checkSumAdjustment);
    result.magicNumber        = BinReader::convert_int(result.magicNumber);
    result.flags              = BinReader::convert_short(result.flags);
    result.unitsPerEm         = BinReader::convert_short(result.unitsPerEm);

    result.created            = BinReader::convert_long(result.created);
    result.modified           = BinReader::convert_long(result.modified);

    result.xMin               = static_cast<int16_t>(BinReader::convert_short(static_cast<uint16_t>(result.xMin)));
    result.yMin               = static_cast<int16_t>(BinReader::convert_short(static_cast<uint16_t>(result.yMin)));
    result.xMax               = static_cast<int16_t>(BinReader::convert_short(static_cast<uint16_t>(result.xMax)));
    result.yMax               = static_cast<int16_t>(BinReader::convert_short(static_cast<uint16_t>(result.yMax)));

    result.macStyle           = BinReader::convert_short(result.macStyle);
    result.lowestRecPPEM      = BinReader::convert_short(result.lowestRecPPEM);
    result.fontDirectionHint  = static_cast<int16_t>(BinReader::convert_short(static_cast<uint16_t>(result.fontDirectionHint)));
    result.indexToLocFormat   = static_cast<int16_t>(BinReader::convert_short(static_cast<uint16_t>(result.indexToLocFormat)));
    result.glyphDataFormat    = static_cast<int16_t>(BinReader::convert_short(static_cast<uint16_t>(result.glyphDataFormat)));

    return result;
}

FontParser::MaxpTable FontParser::parse_maxp_table(std::ifstream& stream) {
    MaxpTable result{};
    stream.read(reinterpret_cast<char*>(&result), sizeof(MaxpTable));

    result.version               = BinReader::convert_int(result.version);
    result.numGlyphs             = BinReader::convert_short(result.numGlyphs);
    result.maxPoints             = BinReader::convert_short(result.maxPoints);
    result.maxContours           = BinReader::convert_short(result.maxContours);
    result.maxCompositePoints    = BinReader::convert_short(result.maxCompositePoints);
    result.maxCompositeContours  = BinReader::convert_short(result.maxCompositeContours);
    result.maxZones              = BinReader::convert_short(result.maxZones);
    result.maxTwilightPoints     = BinReader::convert_short(result.maxTwilightPoints);
    result.maxStorage            = BinReader::convert_short(result.maxStorage);
    result.maxFunctionDefs       = BinReader::convert_short(result.maxFunctionDefs);
    result.maxInstructionDefs    = BinReader::convert_short(result.maxInstructionDefs);
    result.maxStackElements      = BinReader::convert_short(result.maxStackElements);
    result.maxSizeOfInstructions = BinReader::convert_short(result.maxSizeOfInstructions);
    result.maxComponentElements  = BinReader::convert_short(result.maxComponentElements);
    result.maxComponentDepth     = BinReader::convert_short(result.maxComponentDepth);

    return result;
}

std::vector<uint32_t> FontParser::parse_loca_table(std::ifstream &stream, const uint32_t offset, const uint16_t numGlyphs) {
    stream.seekg(offset, std::ios::beg);
    std::vector<uint32_t> result(numGlyphs + 1);
    stream.read(reinterpret_cast<char*>(result.data()), (numGlyphs + 1) * 4);
    for (int i = 0; i < numGlyphs + 1; ++i) {
        result[i] = BinReader::convert_int(result[i]);
    }
    return result;
}

FontParser::CmapHeader FontParser::parse_cmap_header(std::ifstream &stream, const uint32_t offset) {
    stream.seekg(offset, std::ios::beg);
    CmapHeader result{};
    stream.read(reinterpret_cast<char*>(&result), sizeof(CmapHeader));
    result.numSubtables = BinReader::convert_short(result.numSubtables);
    result.version = BinReader::convert_short(result.version);
    return result;
}

FontParser::CmapSubtableHeader parse_cmap_subtable_header(std::ifstream &stream) {
    FontParser::CmapSubtableHeader result{};
    stream.read(reinterpret_cast<char*>(&result), sizeof(FontParser::CmapSubtableHeader));

    result.platformID = BinReader::convert_short(result.platformID);
    result.encodingID = BinReader::convert_short(result.encodingID);
    result.offset     = BinReader::convert_int(result.offset);

    return result;
}

uint32_t FontParser::find_unicode_subtable_offset(std::ifstream &stream, const uint32_t cmap_start_offset, const uint16_t numSubtables) {
    for (int i = 0; i < numSubtables; ++i) {
        if (auto [platformID, encodingID, offset] = parse_cmap_subtable_header(stream);(platformID == 3 && encodingID == 1) || platformID == 0)
            return cmap_start_offset + offset;
    }
    return 0;
}

FontParser::CmapFormat4Header parse_cmap_format4_header(std::ifstream &stream) {
    FontParser::CmapFormat4Header result{};
    stream.read(reinterpret_cast<char*>(&result), sizeof(FontParser::CmapFormat4Header));
    result.format        = BinReader::convert_short(result.format);
    result.length        = BinReader::convert_short(result.length);
    result.language      = BinReader::convert_short(result.language);
    result.segCountX2    = BinReader::convert_short(result.segCountX2);
    result.searchRange   = BinReader::convert_short(result.searchRange);
    result.entrySelector = BinReader::convert_short(result.entrySelector);
    result.rangeShift    = BinReader::convert_short(result.rangeShift);
    return result;
}

FontParser::CmapFormat4Data FontParser::read_cmap_format4_data(std::ifstream &stream) {
    const auto map_header = parse_cmap_format4_header(stream);
    std::cout << map_header << std::endl;
    CmapFormat4Data result;
    result.endCodes = BinReader::read_short_array(stream, map_header.segCountX2 / 2);
    stream.ignore(2);
    result.startCodes = BinReader::read_short_array(stream, map_header.segCountX2 / 2);
    result.idDeltas = BinReader::read_short_array(stream, map_header.segCountX2 / 2);
    result.rangeOffsetsPos = static_cast<int>(stream.tellg());
    result.idRangeOffsets = BinReader::read_short_array(stream, map_header.segCountX2 / 2);
    result.glyphIdArrayOffset = static_cast<int>(stream.tellg());
    result.num_segments = map_header.segCountX2 / 2;
    return result;
}

uint16_t FontParser::get_glyph_id(std::ifstream &stream, const CmapFormat4Data &map_data, const char32_t code) {
    uint16_t min = 0;
    uint16_t max = map_data.num_segments - 1;
    while (min <= max) {
        if (const int mid = (min + max) / 2; map_data.startCodes[mid] <= code) {
            min = mid + 1;
        } else {
            max = mid - 1;
        }
    }
    const int i = min - 1;
    const uint16_t start_code = map_data.startCodes[i];
    const uint16_t end_code = map_data.endCodes[i];
    if (code > end_code) {
        // THROW_RUNTIME("Invalid code");
        return 0;
    }
    const uint16_t id_range_offset = map_data.idRangeOffsets[i];
    const uint16_t id_delta = map_data.idDeltas[i];
    if (id_range_offset == 0) {
        return id_delta + code;
    }
    const auto glyph_offset = id_range_offset + map_data.rangeOffsetsPos + 2 * (i + code - start_code);
    stream.seekg(glyph_offset);
    if (uint16_t glyph_id = BinReader::read_short(stream); glyph_id != 0)
        return glyph_id + static_cast<int16_t>(id_delta);
    return 0;
}

FontParser::GlyphHeader FontParser::parse_glyph_header(std::ifstream& stream) {
    GlyphHeader table{};
    stream.read(reinterpret_cast<char*>(&table), sizeof(GlyphHeader));
    table.numberOfContours = static_cast<int16_t>(BinReader::convert_short(table.numberOfContours));
    table.xMin = static_cast<int16_t>(BinReader::convert_short(table.xMin));
    table.yMin = static_cast<int16_t>(BinReader::convert_short(table.yMin));
    table.xMax = static_cast<int16_t>(BinReader::convert_short(table.xMax));
    table.yMax = static_cast<int16_t>(BinReader::convert_short(table.yMax));
    return table;
}



struct Flag {
    uint8_t flags;
    [[nodiscard]] bool is_on_curve() const { return (flags & 0x01) != 0; }
    [[nodiscard]] bool x_is_short() const { return (flags & 0x02) != 0; }
    [[nodiscard]] bool y_is_short() const { return (flags & 0x04) != 0; }
    [[nodiscard]] bool has_repeat() const { return (flags & 0x08) != 0; }
    [[nodiscard]] bool x_is_same_or_positive() const { return (flags & 0x10) != 0; }
    [[nodiscard]] bool y_is_same_or_positive() const { return (flags & 0x20) != 0; }
};

std::vector<Flag> read_flags(std::ifstream& stream, uint16_t count) {
    std::vector<Flag> result(count);
    uint16_t i = 0;
    while (i < count) {
        stream.read(reinterpret_cast<char*>(&result[i]), sizeof(Flag));
        if (result[i].has_repeat()) {
            uint8_t num_repeats = BinReader::read_byte(stream);
            num_repeats = std::min(static_cast<uint16_t>(num_repeats), static_cast<uint16_t>(count - i - 1));
            std::fill_n(result.begin() + i + 1, num_repeats, result[i]);
            i += num_repeats;
        } else {
            i++;
        }
    }
    return result;
}

FontParser::GlyphData FontParser::get_glyph_points(std::ifstream &stream, const GlyphHeader &header) {
    GlyphData result;
    const auto end_points = BinReader::read_short_array(stream, header.numberOfContours);
    const uint16_t instruction_length = BinReader::read_short(stream);
    stream.seekg(instruction_length, std::ios::cur);
    const auto num_points = end_points.back() + 1;
    const auto flags = read_flags(stream, num_points);
    std::vector<GlyphPoint> glyph_points(num_points);
    int x = 0;
    int y = 0;
    for (size_t i = 0; i < num_points; ++i) {
        if (flags[i].x_is_short()) {
            if (flags[i].x_is_same_or_positive())
                x += BinReader::read_byte(stream);
            else
                x -= BinReader::read_byte(stream);
        } else if (!flags[i].x_is_same_or_positive()) {
            x += static_cast<int16_t>(BinReader::read_short(stream));
        }
        glyph_points[i].x = static_cast<int16_t>(x);
        glyph_points[i].on_curve = flags[i].is_on_curve();
    }

    for (size_t i = 0; i < num_points; ++i) {
        if (flags[i].y_is_short()) {
            if (flags[i].y_is_same_or_positive())
                y += BinReader::read_byte(stream);
            else
                y -= BinReader::read_byte(stream);
        } else if (!flags[i].y_is_same_or_positive()) {
            y += static_cast<int16_t>(BinReader::read_short(stream));
        }
        glyph_points[i].y = static_cast<int16_t>(y);
        glyph_points[i].on_curve = flags[i].is_on_curve();
    }
    result.points = glyph_points;
    result.end_points = end_points;

    return result;
}


FontParser::GlyphOutline FontParser::points_to_outline(GlyphData glyph_data) {
    GlyphOutline result{};
    const auto num_contours = glyph_data.end_points.size();
    int begin = 0;

    for (size_t contour = 0; contour < num_contours; ++contour) {
        const int end = glyph_data.end_points[contour];
        int num_points = end - begin + 1;
        std::vector<GlyphPoint> points;
        const auto& p_begin = glyph_data.points[begin];
        const auto& p_end = glyph_data.points[end];
        if (!p_begin.on_curve && p_end.on_curve) {
            points.resize(num_points + 1);
            points[0] = {
                p_end.x,
                p_end.y,
                true
            };
            std::copy_n(glyph_data.points.data() + begin, num_points, points.data() + 1);
            num_points++;
        }
        else if (p_begin.on_curve && !p_end.on_curve) {
            points.resize(num_points + 1);
            points.back() = {
                p_begin.x,
                p_begin.y,
                true
            };
            std::copy_n(glyph_data.points.data() + begin, num_points, points.data());
            num_points++;
        }
        else if (!p_begin.on_curve && !p_end.on_curve) {
            const auto mid = GlyphPoint{
                static_cast<int16_t>((p_begin.x + p_end.x) / 2),
                static_cast<int16_t>((p_begin.y + p_end.y) / 2),
                true
            };
            points.resize(num_points + 2);
            points[0] = mid;
            points.back() = mid;
            std::copy_n(glyph_data.points.data() + begin, num_points, points.data() + 1);
            num_points += 2;
        }
        else {
            points.resize(num_points);
            std::copy_n(glyph_data.points.data() + begin, num_points, points.data());
        }
        int i = 0;
        while (i < num_points) {
            auto* p0 = &points[i];
            auto* p1 = &points[(i + 1) % num_points];
            auto* p2 = &points[(i + 2) % num_points];
            GlyphPoint new_mid{};

            if (p0->on_curve && p1->on_curve) {
                if (p0->x != p1->x || p0->y != p1->y) {
                    result.lines.emplace_back(
                        p0->x, p0->y,
                        p1->x, p1->y
                    );
                }
                ++i;
            }
            else if (p0->on_curve && !p1->on_curve && p2->on_curve) {
                result.curves.emplace_back(
                    Vec2(p0->x, p0->y),
                    Vec2(p1->x, p1->y),
                    Vec2(p2->x, p2->y)
                );
                i+=2;
            }
            else if (p0->on_curve && !p1->on_curve) {
                while (true) {
                    if (p2->on_curve) {
                        result.curves.emplace_back(
                            Vec2(p0->x, p0->y),
                            Vec2(p1->x, p1->y),
                            Vec2(p2->x, p2->y)
                        );
                        i+=2;
                        break;
                    }
                    const auto mid = GlyphPoint{
                        static_cast<int16_t>((p1->x + p2->x) / 2),
                        static_cast<int16_t>((p1->y + p2->y) / 2),
                        true
                    };
                    result.curves.emplace_back(
                        Vec2(p0->x, p0->y),
                        Vec2(p1->x, p1->y),
                        Vec2(mid.x, mid.y)
                    );
                    new_mid = mid;
                    p0 = &new_mid;
                    p1 = p2;
                    ++i;
                    p2 = &points[(i + 2) % num_points];
                }
            }
            else {
                ++i;
            }
        }
        begin = end + 1;
    }

    return result;
}



std::ostream & FontParser::operator<<(std::ostream &os, const FontHeader &header) {
    os << "=== Font Header ===\n"
            << "  Font Version:   0x" << std::hex << std::setw(8) << std::setfill('0') << header.fontVersion << std::dec << "\n"
            << "  Num Tables:     " << header.numTables << "\n"
            << "  Search Range:   " << header.searchRange << "\n"
            << "  Entry Selector: " << header.entrySelector << "\n"
            << "  Range Shift:    " << header.rangeShift << "\n";
    return os;
}

std::ostream & FontParser::operator<<(std::ostream &os, const TableInfo &table) {
    os << "=== Table Info: ";
    for (const char i : table.title) {
        os << (i ? i : ' ');
    }
    os << " ===\n"
            << "  Checksum:       0x" << std::hex << std::setw(8) << std::setfill('0') << table.checkSum << std::dec << "\n"
            << "  Offset:         " << table.offset << " Bytes\n"
            << "  Length:         " << table.length << " Bytes\n";
    return os;
}

std::ostream& FontParser::operator<<(std::ostream& os, const HeadTable& table) {
    os << "=== Head Table (Font Header) ===\n"
            << "  Version:        " << table.majorVersion << "." << table.minorVersion << "\n"
            << "  Font Revision:  0x" << std::hex << std::setw(8) << std::setfill('0') << table.fontRevision << std::dec << "\n"
            << "  Magic Number:   0x" << std::hex << table.magicNumber << std::dec
            << (table.magicNumber == 0x5F0F3CF5 ? " (VALID)" : " (INVALID!)") << "\n"
            << "  Flags:          0x" << std::hex << table.flags << std::dec << "\n"
            << "  Units Per EM:   " << table.unitsPerEm << " (Grid Size)\n"
            << "  Created (Raw):  " << table.created << "\n"
            << "  Modified (Raw): " << table.modified << "\n"
            << "  Bounding Box:   [" << table.xMin << ", " << table.yMin << "] to [" << table.xMax << ", " << table.yMax << "]\n"
            << "  Mac Style:      0x" << std::hex << table.macStyle << std::dec << "\n"
            << "  IndexToLoc Fmt: " << table.indexToLocFormat
            << (table.indexToLocFormat == 1 ? " (32-bit offsets)" : " (16-bit offsets)") << "\n"
            << "  Glyph Data Fmt: " << table.glyphDataFormat << "\n";
    return os;
}

std::ostream& FontParser::operator<<(std::ostream& os, const MaxpTable& table) {
    os << "=== Maxp Table (Maximum Profile) ===\n"
            << "  Table Version:  0x" << std::hex << table.version << std::dec << "\n"
            << "  Num Glyphs:     " << table.numGlyphs << " (Total unique characters/shapes)\n"
            << "  Max Contours:   " << table.maxContours << "\n"
            << "  Max Points:     " << table.maxPoints << "\n";
    return os;
}

std::ostream& FontParser::operator<<(std::ostream& os, const CmapFormat4Header& header) {
    os << "=== Cmap Subtable Header (Format 4) ===\n"
       << "  Format:         " << header.format << " (Segment Mapping for BMP)\n"
       << "  Subtable Length:" << header.length << " Bytes\n"
       << "  Language ID:    " << header.language << "\n"
       << "  Segment Count:  " << (header.segCountX2 / 2) << " (Raw segCountX2: " << header.segCountX2 << ")\n"
       << "  Search Range:   " << header.searchRange << "\n"
       << "  Entry Selector: " << header.entrySelector << "\n"
       << "  Range Shift:    " << header.rangeShift << "\n";
    return os;
}


std::ostream& FontParser::operator<<(std::ostream& os, const FontParser::GlyphHeader& table) {
    os << "=== Glyph Header ===\n"
       << "  Number of Contours: " << table.numberOfContours << '\n'
       << "  Bounding Box: ["
       << table.xMin << ", "
       << table.yMin << "] to ["
       << table.xMax << ", "
       << table.yMax << "]";

    return os;
}
