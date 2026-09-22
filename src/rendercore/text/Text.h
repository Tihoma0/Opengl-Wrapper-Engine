#ifndef TEXT_H
#define TEXT_H
#include <utility>
#include "Font.h"


class Text {
public:
    std::string text;
    BitmapFont& font;
    int size;

    Text(std::string text, BitmapFont &font, const int size) : text(std::move(text)), font(font), size(size) {}

    void draw(const std::shared_ptr<RenderTarget> &target, const Vec2 &pos, const Color &color) const;
};



#endif //TEXT_H
