
#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <glad/glad.h>

class RenderTarget {
public:
    virtual ~RenderTarget() = default;

    virtual void bind() const = 0;

    [[nodiscard]] virtual GLuint id() const = 0;
    [[nodiscard]] virtual int width() const = 0;
    [[nodiscard]] virtual int height() const = 0;
};
#endif //RENDERTARGET_H
