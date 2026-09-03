#ifndef RENDERBUFFERFORMAT_H
#define RENDERBUFFERFORMAT_H
#include <glad/glad.h>
#include <string>
#include <stdexcept>

enum class PixelStorageFormat {
    R8, RG8, RGB8, RGBA8,
    SRGB8, SRGBA8,

    R16F, RG16F, RGB16F, RGBA16F,

    DEPTH16, DEPTH24, DEPTH32F,
    DEPTH24_STENCIL8, STENCIL8
};

enum class FormatRole {
    Color,
    Depth,
    Stencil,
    DepthStencil
};



inline std::string to_string(const PixelStorageFormat fmt) {
    switch (fmt) {
        case PixelStorageFormat::R8:                return "R8";
        case PixelStorageFormat::RG8:               return "RG8";
        case PixelStorageFormat::RGB8:              return "RGB8";
        case PixelStorageFormat::RGBA8:             return "RGBA8";

        case PixelStorageFormat::SRGB8:             return "SRGB8";
        case PixelStorageFormat::SRGBA8:            return "SRGBA8";

        case PixelStorageFormat::R16F:              return "R16F";
        case PixelStorageFormat::RG16F:             return "RG16F";
        case PixelStorageFormat::RGB16F:            return "RGB16F";
        case PixelStorageFormat::RGBA16F:           return "RGBA16F";

        case PixelStorageFormat::DEPTH16:           return "DEPTH16";
        case PixelStorageFormat::DEPTH24:           return "DEPTH24";
        case PixelStorageFormat::DEPTH32F:          return "DEPTH32F";

        case PixelStorageFormat::DEPTH24_STENCIL8: return "DEPTH24_STENCIL8";

        case PixelStorageFormat::STENCIL8:          return "STENCIL8";

        default:
            return "UNKNOWN";
    }
}

inline FormatRole get_role(const PixelStorageFormat fmt) {
    switch (fmt) {
        case PixelStorageFormat::R8:
        case PixelStorageFormat::RG8:
        case PixelStorageFormat::RGB8:
        case PixelStorageFormat::RGBA8:
        case PixelStorageFormat::SRGB8:
        case PixelStorageFormat::SRGBA8:
        case PixelStorageFormat::R16F:
        case PixelStorageFormat::RG16F:
        case PixelStorageFormat::RGB16F:
        case PixelStorageFormat::RGBA16F:
            return FormatRole::Color;

        case PixelStorageFormat::DEPTH16:
        case PixelStorageFormat::DEPTH24:
        case PixelStorageFormat::DEPTH32F:
            return FormatRole::Depth;

        case PixelStorageFormat::STENCIL8:
            return FormatRole::Stencil;

        case PixelStorageFormat::DEPTH24_STENCIL8:
            return FormatRole::DepthStencil;

        default:
            throw std::runtime_error("Unknown PixelStorageFormat");
    }
}

inline bool has_depth(FormatRole r) {
    return r == FormatRole::Depth || r == FormatRole::DepthStencil;
}

inline bool has_stencil(FormatRole r) {
    return r == FormatRole::Stencil || r == FormatRole::DepthStencil;
}

inline GLint to_gl_internal_format(PixelStorageFormat fmt) {
    switch (fmt) {
        case PixelStorageFormat::R8:      return GL_R8;
        case PixelStorageFormat::RG8:     return GL_RG8;
        case PixelStorageFormat::RGB8:    return GL_RGB8;
        case PixelStorageFormat::RGBA8:   return GL_RGBA8;
        case PixelStorageFormat::SRGB8:   return GL_SRGB8;
        case PixelStorageFormat::SRGBA8: return GL_SRGB8_ALPHA8;
        case PixelStorageFormat::R16F:    return GL_R16F;
        case PixelStorageFormat::RG16F:   return GL_RG16F;
        case PixelStorageFormat::RGB16F:  return GL_RGB16F;
        case PixelStorageFormat::RGBA16F: return GL_RGBA16F;
        case PixelStorageFormat::DEPTH16:  return GL_DEPTH_COMPONENT16;
        case PixelStorageFormat::DEPTH24:  return GL_DEPTH_COMPONENT24;
        case PixelStorageFormat::DEPTH32F: return GL_DEPTH_COMPONENT32F;
        case PixelStorageFormat::DEPTH24_STENCIL8:
            return GL_DEPTH24_STENCIL8;
        case PixelStorageFormat::STENCIL8:
            return GL_STENCIL_INDEX8;
        default:
            throw std::runtime_error("Unknown internal format");
    }
}

inline GLenum to_gl_external_format(PixelStorageFormat fmt) {
    switch (fmt) {
        // color
        case PixelStorageFormat::R8:
        case PixelStorageFormat::R16F:
            return GL_RED;

        case PixelStorageFormat::RG8:
        case PixelStorageFormat::RG16F:
            return GL_RG;

        case PixelStorageFormat::RGB8:
        case PixelStorageFormat::RGB16F:
        case PixelStorageFormat::SRGB8:
            return GL_RGB;

        case PixelStorageFormat::RGBA8:
        case PixelStorageFormat::RGBA16F:
        case PixelStorageFormat::SRGBA8:
            return GL_RGBA;

            // depth
        case PixelStorageFormat::DEPTH16:
        case PixelStorageFormat::DEPTH24:
        case PixelStorageFormat::DEPTH32F:
            return GL_DEPTH_COMPONENT;

        case PixelStorageFormat::DEPTH24_STENCIL8:
            return GL_DEPTH_STENCIL;

        case PixelStorageFormat::STENCIL8:
            return GL_STENCIL_INDEX;

        default:
            throw std::runtime_error("Unknown format");
    }
}

inline GLenum to_gl_type(PixelStorageFormat fmt) {
    switch (fmt) {
        case PixelStorageFormat::R8:
        case PixelStorageFormat::RG8:
        case PixelStorageFormat::RGB8:
        case PixelStorageFormat::RGBA8:
        case PixelStorageFormat::SRGB8:
        case PixelStorageFormat::SRGBA8:
        case PixelStorageFormat::STENCIL8:
            return GL_UNSIGNED_BYTE;

        case PixelStorageFormat::R16F:
        case PixelStorageFormat::RG16F:
        case PixelStorageFormat::RGB16F:
        case PixelStorageFormat::RGBA16F:
            return GL_HALF_FLOAT;

        case PixelStorageFormat::DEPTH32F:
            return GL_FLOAT;

        case PixelStorageFormat::DEPTH24_STENCIL8:
            return GL_UNSIGNED_INT_24_8;

        case PixelStorageFormat::DEPTH16:
        case PixelStorageFormat::DEPTH24:
            return GL_DEPTH_COMPONENT;

        default:
            throw std::runtime_error("Unknown type");
    }
}

inline int get_bytes_per_channel(PixelStorageFormat fmt) {
    switch (fmt) {
        case PixelStorageFormat::R8:      return 1;
        case PixelStorageFormat::RG8:     return 1;
        case PixelStorageFormat::RGB8:    return 1;
        case PixelStorageFormat::RGBA8:   return 1;

        case PixelStorageFormat::SRGB8:   return 1;
        case PixelStorageFormat::SRGBA8:  return 1;

        case PixelStorageFormat::R16F:    return 2;
        case PixelStorageFormat::RG16F:   return 2;
        case PixelStorageFormat::RGB16F:  return 2;
        case PixelStorageFormat::RGBA16F: return 2;

        case PixelStorageFormat::DEPTH16: return 2;
        case PixelStorageFormat::DEPTH24: return 3;
        case PixelStorageFormat::DEPTH32F: return 4;

        case PixelStorageFormat::DEPTH24_STENCIL8: return 31;

        case PixelStorageFormat::STENCIL8: return 1;

        default:
            throw std::runtime_error("Unknown or unsupported PixelStorageFormat for byte size calculation");
    }
}

enum class PixelStorageClass {
    Integer,
    Float,
    Depth,
    DepthStencil,
    Stencil
};

inline PixelStorageClass get_class(PixelStorageFormat fmt) {
    switch (fmt) {
        case PixelStorageFormat::R8:
        case PixelStorageFormat::RG8:
        case PixelStorageFormat::RGB8:
        case PixelStorageFormat::RGBA8:
        case PixelStorageFormat::SRGB8:
        case PixelStorageFormat::SRGBA8:
            return PixelStorageClass::Integer;
        case PixelStorageFormat::R16F:
        case PixelStorageFormat::RG16F:
        case PixelStorageFormat::RGB16F:
        case PixelStorageFormat::RGBA16F:
            return PixelStorageClass::Float;
        case PixelStorageFormat::DEPTH16:
        case PixelStorageFormat::DEPTH24:
        case PixelStorageFormat::DEPTH32F:
            return PixelStorageClass::Depth;
        case PixelStorageFormat::DEPTH24_STENCIL8:
            return PixelStorageClass::DepthStencil;
        case PixelStorageFormat::STENCIL8:
            return PixelStorageClass::Stencil;
        default:
            throw std::runtime_error("Unknown or unsupported PixelStorageFormat for byte size calculation");
    }
}

inline int get_transfer_bytes_per_pixel(PixelStorageFormat fmt)
{
    switch (fmt)
    {
        case PixelStorageFormat::R8:      return 1;
        case PixelStorageFormat::RG8:     return 2;
        case PixelStorageFormat::RGB8:    return 3;
        case PixelStorageFormat::RGBA8:   return 4;

        case PixelStorageFormat::SRGB8:   return 3;
        case PixelStorageFormat::SRGBA8:  return 4;

        case PixelStorageFormat::R16F:    return 2;
        case PixelStorageFormat::RG16F:   return 4;
        case PixelStorageFormat::RGB16F:  return 6;
        case PixelStorageFormat::RGBA16F: return 8;

        case PixelStorageFormat::DEPTH16:
        case PixelStorageFormat::DEPTH24:
        case PixelStorageFormat::DEPTH32F:
            return 4;

        case PixelStorageFormat::DEPTH24_STENCIL8:
            return 4;

        case PixelStorageFormat::STENCIL8:
            return 1;
    }

    throw std::runtime_error("Unknown PixelStorageFormat");
}

inline int get_bytes_per_pixel(PixelStorageFormat fmt) {
    switch (fmt) {
        case PixelStorageFormat::R8:      return 1;
        case PixelStorageFormat::RG8:     return 2;
        case PixelStorageFormat::RGB8:    return 3;
        case PixelStorageFormat::RGBA8:   return 4;

        case PixelStorageFormat::SRGB8:   return 3;
        case PixelStorageFormat::SRGBA8:  return 4;

        case PixelStorageFormat::R16F:    return 2;
        case PixelStorageFormat::RG16F:   return 4;
        case PixelStorageFormat::RGB16F:  return 6;
        case PixelStorageFormat::RGBA16F: return 8;

        case PixelStorageFormat::DEPTH16: return 2;
        case PixelStorageFormat::DEPTH24: return 3;
        case PixelStorageFormat::DEPTH32F: return 4;

        case PixelStorageFormat::DEPTH24_STENCIL8: return 4;

        case PixelStorageFormat::STENCIL8: return 1;

        default:
            throw std::runtime_error("Unknown or unsupported PixelStorageFormat for byte size calculation");
    }
}

inline int get_channels(PixelStorageFormat fmt) {
    switch (fmt) {
        case PixelStorageFormat::R8:
        case PixelStorageFormat::R16F:
            return 1;
        case PixelStorageFormat::RG8:
        case PixelStorageFormat::RG16F:
            return 2;
        case PixelStorageFormat::RGB8:
        case PixelStorageFormat::SRGB8:
        case PixelStorageFormat::RGB16F:
            return 3;
        case PixelStorageFormat::RGBA8:
        case PixelStorageFormat::SRGBA8:
        case PixelStorageFormat::RGBA16F:
            return 4;
        default:
            return 0;
    }
}

inline bool same_role(PixelStorageFormat a, PixelStorageFormat b) {
    return get_role(a) == get_role(b);
}

inline bool exact_format_match(PixelStorageFormat dst, PixelStorageFormat src) {
    return dst == src;
}

inline bool subimage_compatible(PixelStorageFormat dst, PixelStorageFormat src) {
    return exact_format_match(dst, src);
}

inline bool upload_compatible(PixelStorageFormat dst, PixelStorageFormat src) {
    if (!same_role(dst, src))
        return false;
    if (exact_format_match(dst, src))
        return true;
    if (get_role(dst) == FormatRole::Color) {
        if (src == PixelStorageFormat::RGB8 &&
            (dst == PixelStorageFormat::RGBA8 ||
             dst == PixelStorageFormat::SRGBA8))
            return true;
        return get_bytes_per_pixel(dst) == get_bytes_per_pixel(src);
    }

    return false;
}

inline bool requires_cpu_convert(PixelStorageFormat dst, PixelStorageFormat src) {
    if (!same_role(dst, src))
        return true;

    if (subimage_compatible(dst, src) || upload_compatible(dst, src))
        return false;

    return true;
}

inline bool incompatible(PixelStorageFormat dst, PixelStorageFormat src) {
    return !same_role(dst, src);
}

#endif //RENDERBUFFERFORMAT_H
