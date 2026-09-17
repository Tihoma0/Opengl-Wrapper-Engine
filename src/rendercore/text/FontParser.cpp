
#include "FontParser.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "BinReader.h"
#include "Tables.h"
#include "rendercore/core/Warnings.h"

void FontParser::parse(std::string path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        THROW_RUNTIME("Error when opening " + path);
    }
    auto fheader = Tables::parse_header(file);
    std::cout << fheader << std::endl;
    auto tables = Tables::parse_tables(file, fheader.numTables);
    auto head_table = Tables::parse_head_table(file);
    std::cout << head_table << std::endl;
    std::cout << tables["maxp"] << std::endl;
    file.seekg(tables["maxp"].offset, std::ios::beg);
    auto maxp_table = Tables::parse_maxp_table(file);
    std::cout << maxp_table << std::endl;

    auto cmapHeader = Tables::parse_cmap_header(file, tables["cmap"].offset);
    auto globalSubtableOffset = Tables::find_unicode_subtable_offset(file, tables["cmap"].offset, cmapHeader.numSubtables);

    if (globalSubtableOffset != 0) {
        file.seekg(globalSubtableOffset, std::ios::beg);

        uint16_t format = 0;
        file.read(reinterpret_cast<char*>(&format), sizeof(format));
        format = BinReader::convert_short(format);

        std::cout << "Unicode-Subtabelle gefunden bei Offset: " << globalSubtableOffset << "\n";
        std::cout << "Cmap Datenformat: Format " << format << "\n";

        auto map_data = Tables::read_cmap_format4_data(file);
    } else {
        std::cerr << "Kritischer Fehler: Keine unterstützte Unicode-cmap-Tabelle gefunden!\n";
    }
}
