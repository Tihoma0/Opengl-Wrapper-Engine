#ifndef FRAMEBUFFERATTACHMENT_H
#define FRAMEBUFFERATTACHMENT_H
#include "glad/glad.h"
#include "rendercore/core/enums/textures/PixelStorageFormat.h"
enum class AttachmentType {
    Texture,
    Renderbuffer
};
class FramebufferAttachment {
public:
    virtual ~FramebufferAttachment() = default;

    virtual void bind() const = 0;
    virtual GLuint id() const = 0;
    virtual PixelStorageFormat format() const = 0;
    virtual int width() const = 0;
    virtual int height() const = 0;
    virtual void resize(int width, int height) = 0;
    virtual AttachmentType type() const = 0;
};
#endif //FRAMEBUFFERATTACHMENT_H
