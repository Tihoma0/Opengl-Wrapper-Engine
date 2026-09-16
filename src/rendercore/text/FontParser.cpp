
#include "FontParser.h"

#include <fstream>
#include <iostream>

#include "BinReader.h"
#include "Tables.h"
#include "rendercore/core/Warnings.h"

void FontParser::parse(std::string path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        THROW_RUNTIME("Error when opening " + path);
    }
    Tables::FontHeader fheader{};

    fheader.fontVersion = BinReader::read_int(file);
    std::cout << "Font version: " << fheader.fontVersion << std::endl;

}
