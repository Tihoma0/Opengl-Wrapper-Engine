#ifndef DRAWMODE_H
#define DRAWMODE_H
#include "glad/glad.h"
enum class DrawMode {
    STATIC_DRAW,
    DYNAMIC_DRAW
};

inline GLuint to_gl(DrawMode mode) {
    switch (mode) {
        case DrawMode::STATIC_DRAW: return GL_STATIC_DRAW;
        case DrawMode::DYNAMIC_DRAW: return GL_DYNAMIC_DRAW;
        default: return GL_STATIC_DRAW;
    }
}
#endif //DRAWMODE_H
