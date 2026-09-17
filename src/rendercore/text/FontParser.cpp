
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
}
