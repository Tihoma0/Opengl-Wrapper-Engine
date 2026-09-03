#ifndef PIXEL_H
#define PIXEL_H

#include <vector>

#include "../core/enums/textures/PixelStorageFormat.h"


class Color {

public:

	static Color from_buffer(const uint8_t* src, PixelStorageFormat fmt);

    float r = 1.0f, g = 0.0f, b = 0.0f, a = 1.0f;

	Color() = default;

    explicit Color(unsigned int color);

	bool operator==(const Color & o) const {
	    return r == o.r && g == o.g && b == o.b && a == o.a;
    }

	Color(int r, int g, int b, int a = 255);

	Color(float r, float g, float b, float a = 1.0);

	[[nodiscard]] std::vector<uint8_t> pack(PixelStorageFormat fmt) const;

	void pack(PixelStorageFormat fmt, uint8_t* dst) const;
};

static_assert(offsetof(Color, r) == 0);
static_assert(offsetof(Color, g) == 4);
static_assert(offsetof(Color, b) == 8);
static_assert(offsetof(Color, a) == 12);
static_assert(sizeof(Color) == 16);

#endif //PIXEL_H
