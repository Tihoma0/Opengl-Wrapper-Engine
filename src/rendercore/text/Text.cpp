
#include "Text.h"

#include "rendercore/surface/Color.h"

void Text::draw(const std::shared_ptr<RenderTarget> &target, const Vec2 &pos, const Color &color) const {
    font.draw(target, text, pos, size, color);
}
