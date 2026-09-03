#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H
#include <memory>

#include "rendercore/surface/images/Image.h"
#include "rendercore/core/enums/textures/PixelStorageFormat.h"
#include "glad/glad.h"
#include "rendercore/core/enums/textures/FramebufferAttachment.h"

#include "rendercore/core/Warnings.h"


class Renderbuffer :
    public std::enable_shared_from_this<Renderbuffer>,
    public FramebufferAttachment
{
public:
    
    static std::shared_ptr<Renderbuffer> create(int width, int height, PixelStorageFormat format = PixelStorageFormat::RGBA8, int samples = 0) {
        return std::shared_ptr<Renderbuffer>(new Renderbuffer(width, height, format, samples));
    }

    Renderbuffer(const Renderbuffer&) = delete;
    Renderbuffer& operator=(const Renderbuffer&) = delete;

    Renderbuffer(Renderbuffer&&) noexcept = default;
    Renderbuffer& operator=(Renderbuffer&&) noexcept = default;
    
    ~Renderbuffer() override {
        glDeleteRenderbuffers(1, &m_id);
    }

    void bind() const override {
        glBindRenderbuffer(GL_RENDERBUFFER, m_id);
    }

    void resize(int width, int height) override {
        bind();
        if (m_samples == 0)
            glRenderbufferStorage(GL_RENDERBUFFER, to_gl_internal_format(m_format), width, height);
        else if (m_samples >= 2)
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, to_gl_internal_format(m_format), width, height);
        m_width = width;
        m_height = height;
    }

    [[nodiscard]] int width() const override { return m_width; }
    [[nodiscard]] int height() const override { return m_height; }
    [[nodiscard]] PixelStorageFormat format() const override { return m_format; }
    [[nodiscard]] GLuint id() const override { return m_id; }
    [[nodiscard]] int samples() const { return m_samples; }
    [[nodiscard]] AttachmentType type() const override { return AttachmentType::Renderbuffer; }

private:
    GLuint m_id = 0;
    int m_width = 0;
    int m_height = 0;
    PixelStorageFormat m_format = PixelStorageFormat::RGBA8;
    int m_samples = 0;

    Renderbuffer(int width, int height, PixelStorageFormat format, int samples) {
        glGenRenderbuffers(1, &m_id);
        m_width = width;
        m_height = height;
        m_format = format;
        m_samples = samples;
        Renderbuffer::bind();
        if (samples == 0)
            glRenderbufferStorage(GL_RENDERBUFFER, to_gl_internal_format(format), width, height);
        else if (samples >= 2)
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, to_gl_internal_format(format), width, height);
        else
            THROW_RUNTIME("Invalid number of samples");
    }
};
#endif //RENDERBUFFER_H
