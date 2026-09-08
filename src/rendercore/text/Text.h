#ifndef TEXT_H
#define TEXT_H
#include <utility>

#include "Font.h"


class Text {
public:
    std::string text;
    Font font;
    int size;

    Text(std::string text, const Font &font, const int size) : text(std::move(text)), font(font), size(size) {}
};



#endif //TEXT_H
