#ifndef BLENDFUNC_H
#define BLENDFUNC_H
#include "../core/Warnings.h"
#include "glad/glad.h"

enum class BlendFactor {
    Zero,
    One,

    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,

    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha,

    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha,

    SrcAlphaSaturate,

    Src1Color,
    OneMinusSrc1Color,
    Src1Alpha,
    OneMinusSrc1Alpha
};

enum class BlendEquation {
    Add,
    Subtract,
    ReverseSubtract,
    Min,
    Max
};

inline GLenum to_gl(const BlendFactor factor) {
    switch (factor) {
        case BlendFactor::Zero:                  return GL_ZERO;
        case BlendFactor::One:                   return GL_ONE;

        case BlendFactor::SrcColor:              return GL_SRC_COLOR;
        case BlendFactor::OneMinusSrcColor:      return GL_ONE_MINUS_SRC_COLOR;
        case BlendFactor::DstColor:              return GL_DST_COLOR;
        case BlendFactor::OneMinusDstColor:      return GL_ONE_MINUS_DST_COLOR;

        case BlendFactor::SrcAlpha:              return GL_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha:      return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DstAlpha:              return GL_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha:      return GL_ONE_MINUS_DST_ALPHA;

        case BlendFactor::ConstantColor:         return GL_CONSTANT_COLOR;
        case BlendFactor::OneMinusConstantColor: return GL_ONE_MINUS_CONSTANT_COLOR;
        case BlendFactor::ConstantAlpha:         return GL_CONSTANT_ALPHA;
        case BlendFactor::OneMinusConstantAlpha: return GL_ONE_MINUS_CONSTANT_ALPHA;

        case BlendFactor::SrcAlphaSaturate:      return GL_SRC_ALPHA_SATURATE;

        case BlendFactor::Src1Color:             return GL_SRC1_COLOR;
        case BlendFactor::OneMinusSrc1Color:     return GL_ONE_MINUS_SRC1_COLOR;
        case BlendFactor::Src1Alpha:             return GL_SRC1_ALPHA;
        case BlendFactor::OneMinusSrc1Alpha:     return GL_ONE_MINUS_SRC1_ALPHA;
    }

    THROW_RUNTIME("Invalid BlendFactor");
}

inline GLenum to_gl(const BlendEquation equation) {
    switch (equation) {
        case BlendEquation::Add:             return GL_FUNC_ADD;
        case BlendEquation::Subtract:        return GL_FUNC_SUBTRACT;
        case BlendEquation::ReverseSubtract: return GL_FUNC_REVERSE_SUBTRACT;
        case BlendEquation::Min:             return GL_MIN;
        case BlendEquation::Max:             return GL_MAX;
    }

    THROW_RUNTIME("Invalid BlendEquation");
}

struct BlendState {
    bool enabled = true;

    BlendFactor src_rgb = BlendFactor::SrcAlpha;
    BlendFactor dst_rgb = BlendFactor::OneMinusSrcAlpha;

    BlendFactor src_alpha = BlendFactor::One;
    BlendFactor dst_alpha = BlendFactor::OneMinusSrcAlpha;

    BlendEquation equation_rgb = BlendEquation::Add;
    BlendEquation equation_alpha = BlendEquation::Add;

    void apply() const {
        if (enabled) {
            glEnable(GL_BLEND);

            glBlendFuncSeparate(
                to_gl(src_rgb),
                to_gl(dst_rgb),
                to_gl(src_alpha),
                to_gl(dst_alpha)
            );

            glBlendEquationSeparate(to_gl(equation_rgb), to_gl(equation_alpha));
        }
        else {
            glDisable(GL_BLEND);
        }
    }

    bool operator==(const BlendState & blend_state) const = default;
};

namespace BlendPresets {

    constexpr BlendState Disabled() {
        return {
            .enabled = false
        };
    }

    constexpr BlendState Alpha() {
        return {
            .enabled = true,

            .src_rgb = BlendFactor::SrcAlpha,
            .dst_rgb = BlendFactor::OneMinusSrcAlpha,

            .src_alpha = BlendFactor::One,
            .dst_alpha = BlendFactor::OneMinusSrcAlpha,

            .equation_rgb = BlendEquation::Add,
            .equation_alpha = BlendEquation::Add
        };
    }

    constexpr BlendState PremultipliedAlpha() {
        return {
            .enabled = true,

            .src_rgb = BlendFactor::One,
            .dst_rgb = BlendFactor::OneMinusSrcAlpha,

            .src_alpha = BlendFactor::One,
            .dst_alpha = BlendFactor::OneMinusSrcAlpha,

            .equation_rgb = BlendEquation::Add,
            .equation_alpha = BlendEquation::Add
        };
    }

    constexpr BlendState Additive() {
        return {
            .enabled = true,

            .src_rgb = BlendFactor::SrcAlpha,
            .dst_rgb = BlendFactor::One,

            .src_alpha = BlendFactor::One,
            .dst_alpha = BlendFactor::One,

            .equation_rgb = BlendEquation::Add,
            .equation_alpha = BlendEquation::Add
        };
    }

    constexpr BlendState AdditiveUnattenuated() {
        return {
            .enabled = true,

            .src_rgb = BlendFactor::One,
            .dst_rgb = BlendFactor::One,

            .src_alpha = BlendFactor::One,
            .dst_alpha = BlendFactor::One,

            .equation_rgb = BlendEquation::Add,
            .equation_alpha = BlendEquation::Add
        };
    }

    constexpr BlendState Subtractive() {
        return {
            .enabled = true,

            .src_rgb = BlendFactor::SrcAlpha,
            .dst_rgb = BlendFactor::One,

            .src_alpha = BlendFactor::One,
            .dst_alpha = BlendFactor::One,

            .equation_rgb = BlendEquation::ReverseSubtract,
            .equation_alpha = BlendEquation::Add
        };
    }

    constexpr BlendState Multiply() {
        return {
            .enabled = true,

            .src_rgb = BlendFactor::DstColor,
            .dst_rgb = BlendFactor::Zero,

            .src_alpha = BlendFactor::One,
            .dst_alpha = BlendFactor::OneMinusSrcAlpha,

            .equation_rgb = BlendEquation::Add,
            .equation_alpha = BlendEquation::Add
        };
    }

    constexpr BlendState Screen() {
        return {
            .enabled = true,

            .src_rgb = BlendFactor::One,
            .dst_rgb = BlendFactor::OneMinusSrcColor,

            .src_alpha = BlendFactor::One,
            .dst_alpha = BlendFactor::OneMinusSrcAlpha,

            .equation_rgb = BlendEquation::Add,
            .equation_alpha = BlendEquation::Add
        };
    }

    constexpr BlendState Max() {
        return {
            .enabled = true,

            .src_rgb = BlendFactor::One,
            .dst_rgb = BlendFactor::One,

            .src_alpha = BlendFactor::One,
            .dst_alpha = BlendFactor::One,

            .equation_rgb = BlendEquation::Max,
            .equation_alpha = BlendEquation::Max
        };
    }

    constexpr BlendState Min() {
        return {
            .enabled = true,

            .src_rgb = BlendFactor::One,
            .dst_rgb = BlendFactor::One,

            .src_alpha = BlendFactor::One,
            .dst_alpha = BlendFactor::One,

            .equation_rgb = BlendEquation::Min,
            .equation_alpha = BlendEquation::Min
        };
    }

}
#endif //BLENDFUNC_H
