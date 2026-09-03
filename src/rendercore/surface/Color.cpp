#include "Color.h"

#include "algorithm"
#include <cstring>
#include <iostream>
#include <vector>

#include "../core/enums/textures/PixelStorageFormat.h"
#include "rendercore/core/math/type_conversion/TypeConversion.h"


Color Color::from_buffer(const uint8_t* src, const PixelStorageFormat fmt) {
	Color result;

	const int num_channels = get_channels(fmt);
	switch (get_class(fmt)) {
		case PixelStorageClass::Integer: {
			float* rgba = &result.r;
			for (int i = 0; i < num_channels; ++i) {
				rgba[i] = u8_to_f32(src[i]);
			}
			break;
		}
		case PixelStorageClass::Float: {
			const int bytes_per_channel = get_bytes_per_channel(fmt);

			float* rgba = &result.r;
			if (bytes_per_channel == 2)
			{
				for (int i = 0; i < num_channels; ++i) {
					uint16_t h;
					memcpy(&h, src + i * 2, 2);
					rgba[i] = f16_to_f32(h);
				}
			}
			else
			{
				memcpy(rgba, src, get_bytes_per_pixel(fmt));
			}

			break;
		}
		case PixelStorageClass::Depth:
		{
			switch(fmt)
			{
				case PixelStorageFormat::DEPTH16:
				{
					uint16_t d;
					memcpy(&d, src, 2);
					result.r = static_cast<float>(d) / 65535.0f;
					result.g = result.r;
					result.b = result.r;
					break;
				}
				case PixelStorageFormat::DEPTH24:
				{
					uint32_t d = 0;
					memcpy(&d, src, 3);
					result.r = static_cast<float>(d) / 16777215.0f;
					result.g = result.r;
					result.b = result.r;
					break;
				}
				case PixelStorageFormat::DEPTH32F:
				{
					memcpy(&result.r, src, 4);
					result.g = result.r;
					result.b = result.r;
					break;
				}
				default: break;
			}
			break;
		}
		case PixelStorageClass::DepthStencil: {
			uint32_t d = 0;
			memcpy(&d, src, 3);
			result.r = static_cast<float>(d) / 16777215.0f;
			uint8_t s;
			memcpy(&s, src + 3, 1);
			result.g = static_cast<float>(s) / 255.0f;
			std::cout << "Depth: " << result.r << " Stencil: " << result.g << std::endl;
			break;
		}
		case PixelStorageClass::Stencil: // only stencil8
		{
			const float v = u8_to_f32(src[0]);
			result.r = v;
			result.g = v;
			result.b = v;
			break;
		}
	}
	return result;
}



Color::Color(const unsigned int color) {
	r = static_cast<float>(color >> 24 & 0xff) / 255.0f;
	g = static_cast<float>(color >> 16 & 0xff) / 255.0f;
	b = static_cast<float>(color >> 8 & 0xff) / 255.0f;
	a = static_cast<float>(color & 0xff) / 255.0f;
}


Color::Color(const int r, const int g, const int b, const int a) {
	this->r = static_cast<float>(r) / 255.0f;
	this->g = static_cast<float>(g) / 255.0f;
	this->b = static_cast<float>(b) / 255.0f;
	this->a = static_cast<float>(a) / 255.0f;
}

Color::Color(const float r, const float g, const float b, const float a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

[[nodiscard]] std::vector<uint8_t> Color::pack(const PixelStorageFormat fmt) const {
    std::vector<uint8_t> out;
    int channels = get_channels(fmt);
    out.reserve(get_bytes_per_pixel(fmt));

    auto write_u8 = [&](const float v) {
    	const uint8_t x = f32_to_u8(v);
    	out.push_back(x);
    };

    auto write_f16 = [&](const float v) {
    	const uint16_t x = f32_to_f16(v);
    	const auto* bytes = reinterpret_cast<const uint8_t*>(&x);
    	out.insert(out.end(), bytes, bytes + 2);
    };

    auto write_f32 = [&](const float v) {
    	const auto* bytes = reinterpret_cast<const uint8_t*>(&v);
    	out.insert(out.end(), bytes, bytes + 4);
    };

    switch (get_class(fmt)) {
    	case PixelStorageClass::Integer: {
    		const float* rgba = &r;
    		for (int i = 0; i < channels; i++) {
    			write_u8(rgba[i]); // there are only 8-bit integer formats
    		}
    		break;
    	}
    	case PixelStorageClass::Float: {
    		if (get_bytes_per_channel(fmt) == 2) {
    			const float* rgba = &r;
    			for (int i = 0; i < channels; i++) {
    				write_f16(rgba[i]);
    			}
    		}
    		else {
    			const float* rgba = &r;

    			const int bytes = channels * static_cast<int>(sizeof(float));
    			out.resize(bytes);
    			std::memcpy(out.data(), rgba, bytes);
    		}
    			break;
    	}
    	case PixelStorageClass::Depth: {
    		switch (fmt) {
    			case PixelStorageFormat::DEPTH16: {
    				uint16_t v =
					static_cast<uint16_t>(
						std::clamp(r, 0.0f, 1.0f) * 65535.0f
					);
    				out.resize(2);
    				out[0] = v;
    				out[1] = v >> 8;
    				break;
    			}
    			case PixelStorageFormat::DEPTH24: {
    				const uint32_t v = static_cast<uint32_t>(std::clamp(r, 0.0f, 1.0f) * 16777215.0f);
    				out.resize(3);
    				out[0] = v;
    				out[1] = v >> 8;
    				out[2] = v >> 16;
    				break;
    			}
    			case PixelStorageFormat::DEPTH32F: {
    				write_f32(r);
    				break;
    			}
			    default: break;
		    }
    		break;
    	}
    	case PixelStorageClass::DepthStencil: {
    		uint32_t d = static_cast<uint32_t>(std::clamp(r, 0.0f, 1.0f) * 16777215.0f);
    		uint8_t s = f32_to_u8(g);
    		out.resize(4);
    		out[0] = d;
    		out[1] = d >> 8;
    		out[2] = d >> 16;
    		out[3] = s;
    		break;
    	}
    	case PixelStorageClass::Stencil:
    		write_u8(r);
    		break;
    }
    return out;
}



void Color::pack(const PixelStorageFormat fmt, uint8_t* dst) const {
    const int channels = get_channels(fmt);

    auto write_u8 = [&](float v) {
    	*dst++ = f32_to_u8(v);
    };

    auto write_f16 = [&](const float v)
    {
    	const uint16_t x = f32_to_f16(v);
    	std::memcpy(dst, &x, 2);
    	dst += 2;
    };

    auto write_f32 = [&](const float v)
    {
    	std::memcpy(dst, &v, 4);
    	dst += 4;
    };

    switch (get_class(fmt)) {
    	case PixelStorageClass::Integer: {
    		const float* rgba = &r;
    		for (int i = 0; i < channels; i++) {
    			write_u8(rgba[i]); // there are only 8-bit integer formats
    		}
    		break;
    	}
    	case PixelStorageClass::Float: {
    		if (get_bytes_per_channel(fmt) == 2) {
    			const float* rgba = &r;
    			for (int i = 0; i < channels; i++) {
    				write_f16(rgba[i]);
    			}
    		}
    		else
    			memcpy(dst, &r, get_bytes_per_pixel(fmt));
    		break;
    	}
    	case PixelStorageClass::Depth: {
    		switch (fmt) {
    			case PixelStorageFormat::DEPTH16: {
    				uint16_t v =
					static_cast<uint16_t>(
						std::clamp(r, 0.0f, 1.0f) * 65535.0f
					);
    				*dst++ = v;
    				*dst = v >> 8;
    				break;
    			}
    			case PixelStorageFormat::DEPTH24: {
    				const uint32_t v = static_cast<uint32_t>(std::clamp(r, 0.0f, 1.0f) * 16777215.0f);
    				*dst++ = v;
    				*dst++ = v >> 8;
    				*dst = v >> 16;
    				break;
    			}
    			case PixelStorageFormat::DEPTH32F: {
    				write_f32(r);
    				break;
    			}
    			default: break;
    		}
    		break;
    	}
    	case PixelStorageClass::DepthStencil: {
    		uint32_t d = static_cast<uint32_t>(std::clamp(r, 0.0f, 1.0f) * 16777215.0f);
    		uint8_t s = f32_to_u8(g);
    		*dst++ = d;
    		*dst++ = d >> 8;
    		*dst++ = d >> 16;
    		*dst = s;
    		break;
    	}
    	case PixelStorageClass::Stencil:
    		write_u8(r);
    		break;
    }
}

