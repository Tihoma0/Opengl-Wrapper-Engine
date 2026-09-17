//
// Created by Yanran on 16/09/2026.
//

#include "Tables.h"

#include <fstream>
#include <iomanip>
#include <iosfwd>
#include <iostream>

#include "BinReader.h"

Tables::FontHeader Tables::parse_header(std::ifstream& stream) {
    FontHeader result{};
    stream.read(reinterpret_cast<char*>(&result), sizeof(FontHeader));
    result.fontVersion = BinReader::convert_int(result.fontVersion);
    result.numTables = BinReader::convert_short(result.numTables);
    result.searchRange = BinReader::convert_short(result.searchRange);
    result.entrySelector = BinReader::convert_short(result.entrySelector);
    result.rangeShift = BinReader::convert_short(result.rangeShift);
    return result;
}

Tables::TableInfo Tables::parse_table(std::ifstream &stream) {
    TableInfo result{};
    stream.read(reinterpret_cast<char*>(&result), sizeof(TableInfo));
    result.offset = BinReader::convert_int(result.offset);
    result.length = BinReader::convert_int(result.length);
    return result;
}


std::unordered_map<std::string, Tables::TableInfo> Tables::parse_tables(std::ifstream &stream, const int num_tables) {
    std::unordered_map<std::string, TableInfo> result;
    for (int i = 0; i < num_tables; ++i) {
        auto table = parse_table(stream);
        std::string key(table.title, 4);
        result[key] = table;
    }
    return result;
}

Tables::HeadTable Tables::parse_head_table(std::ifstream &stream) {
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

Tables::MaxpTable Tables::parse_maxp_table(std::ifstream& stream) {
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

std::vector<uint32_t> Tables::parse_loca_table(std::ifstream &stream, const uint32_t offset, const uint16_t numGlyphs) {
    stream.seekg(offset, std::ios::beg);
    std::vector<uint32_t> result(numGlyphs + 1);
    stream.read(reinterpret_cast<char*>(result.data()), (numGlyphs + 1) * 4);
    for (int i = 0; i < numGlyphs + 1; ++i) {
        result[i] = BinReader::convert_int(result[i]);
    }
    return result;
}

Tables::CmapHeader Tables::parse_cmap_header(std::ifstream &stream, const uint32_t offset) {
    stream.seekg(offset, std::ios::beg);
    CmapHeader result{};
    stream.read(reinterpret_cast<char*>(&result), sizeof(CmapHeader));
    result.numSubtables = BinReader::convert_short(result.numSubtables);
    result.version = BinReader::convert_short(result.version);
    return result;
}

Tables::CmapSubtableHeader Tables::parse_cmap_subtable_header(std::ifstream &stream) {
    CmapSubtableHeader result{};
    stream.read(reinterpret_cast<char*>(&result), sizeof(CmapSubtableHeader));

    result.platformID = BinReader::convert_short(result.platformID);
    result.encodingID = BinReader::convert_short(result.encodingID);
    result.offset     = BinReader::convert_int(result.offset);

    return result;
}

uint32_t Tables::find_unicode_subtable_offset(std::ifstream &stream, const uint32_t cmap_start_offset, const uint16_t numSubtables) {
    for (int i = 0; i < numSubtables; ++i) {
        if (auto [platformID, encodingID, offset] = parse_cmap_subtable_header(stream);(platformID == 3 && encodingID == 1) || platformID == 0)
            return cmap_start_offset + offset;
    }
    return 0;
}

Tables::CmapFormat4Header Tables::parse_cmap_format4_header(std::ifstream &stream) {
    CmapFormat4Header result{};
    stream.read(reinterpret_cast<char*>(&result), sizeof(CmapFormat4Header));
    result.length        = BinReader::convert_short(result.length);
    result.language      = BinReader::convert_short(result.language);
    result.segCountX2    = BinReader::convert_short(result.segCountX2);
    result.searchRange   = BinReader::convert_short(result.searchRange);
    result.entrySelector = BinReader::convert_short(result.entrySelector);
    result.rangeShift    = BinReader::convert_short(result.rangeShift);
    return result;
}

Tables::CmapFormat4Data Tables::read_cmap_format4_data(std::ifstream &stream) {
    const auto map_header = parse_cmap_format4_header(stream);
    std::cout << map_header << std::endl;
    CmapFormat4Data result;
    result.endCodes = BinReader::read_short_array(stream, map_header.segCountX2 / 2);
    stream.ignore(2);
    result.startCodes = BinReader::read_short_array(stream, map_header.segCountX2 / 2);
    result.idDeltas = BinReader::read_short_array(stream, map_header.segCountX2 / 2);
    result.idRangeOffsets = BinReader::read_short_array(stream, map_header.segCountX2 / 2);
    result.glyphIdArrayOffset = stream.tellg();
    return result;
}


std::ostream & Tables::operator<<(std::ostream &os, const FontHeader &header) {
    os << "=== Font Header ===\n"
            << "  Font Version:   0x" << std::hex << std::setw(8) << std::setfill('0') << header.fontVersion << std::dec << "\n"
            << "  Num Tables:     " << header.numTables << "\n"
            << "  Search Range:   " << header.searchRange << "\n"
            << "  Entry Selector: " << header.entrySelector << "\n"
            << "  Range Shift:    " << header.rangeShift << "\n";
    return os;
}

std::ostream & Tables::operator<<(std::ostream &os, const TableInfo &table) {
    os << "=== Table Info: ";
    for (int i = 0; i < 4; ++i) {
        os << (table.title[i] ? table.title[i] : ' ');
    }
    os << " ===\n"
            << "  Checksum:       0x" << std::hex << std::setw(8) << std::setfill('0') << table.checkSum << std::dec << "\n"
            << "  Offset:         " << table.offset << " Bytes\n"
            << "  Length:         " << table.length << " Bytes\n";
    return os;
}

std::ostream& Tables::operator<<(std::ostream& os, const HeadTable& table) {
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

std::ostream& Tables::operator<<(std::ostream& os, const MaxpTable& table) {
    os << "=== Maxp Table (Maximum Profile) ===\n"
            << "  Table Version:  0x" << std::hex << table.version << std::dec << "\n"
            << "  Num Glyphs:     " << table.numGlyphs << " (Total unique characters/shapes)\n"
            << "  Max Contours:   " << table.maxContours << "\n"
            << "  Max Points:     " << table.maxPoints << "\n";
    return os;
}

std::ostream& Tables::operator<<(std::ostream& os, const CmapFormat4Header& header) {
    os << "=== Cmap Subtable Header (Format 4) ===\n"
       << "  Format:         4 (Segment Mapping for BMP)\n"
       << "  Subtable Length:" << header.length << " Bytes\n"
       << "  Language ID:    " << header.language << "\n"
       << "  Segment Count:  " << (header.segCountX2 / 2) << " (Raw segCountX2: " << header.segCountX2 << ")\n"
       << "  Search Range:   " << header.searchRange << "\n"
       << "  Entry Selector: " << header.entrySelector << "\n"
       << "  Range Shift:    " << header.rangeShift << "\n";
    return os;
}
