#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <iostream>
#include <memory>
#include <unordered_map>

#include "rendercore/core/enums/textures/FramebufferAttachment.h"
#include "glad/glad.h"
#include "RenderTarget.h"
#include "rendercore/opengl/GLTrack.h"
#include "rendercore/surface/textures/Texture.h"

class Framebuffer : public RenderTarget {
public:

    static std::shared_ptr<Framebuffer> create() {
        return std::shared_ptr<Framebuffer>(new Framebuffer());
    }

    Framebuffer(const Framebuffer&) = delete;
    Framebuffer& operator=(const Framebuffer&) = delete;

    Framebuffer(Framebuffer&&) noexcept = default;
    Framebuffer& operator=(Framebuffer&&) noexcept = default;

    ~Framebuffer() {
        glDeleteFramebuffers(1, &m_fbo);
    }

    void bind() const override {
        bind_framebuffer(m_fbo);
        glViewport(0, 0, m_width, m_height);
    }

    void resize(int width, int height);


    void add_color_attachment(const std::shared_ptr<FramebufferAttachment> &attachment, int slot);

    void add_depth_attachment(const std::shared_ptr<FramebufferAttachment> &attachment);

    void add_stencil_attachment(const std::shared_ptr<FramebufferAttachment> &attachment);

    void add_depth_stencil_attachment(const std::shared_ptr<FramebufferAttachment> &attachment);

    std::shared_ptr<FramebufferAttachment> detach_depth_attachment();

    std::shared_ptr<FramebufferAttachment> detach_stencil_attachment();

    std::shared_ptr<FramebufferAttachment> detach_depth_stencil_attachment();

    std::shared_ptr<FramebufferAttachment> detach_color_attachment(int slot);

    [[nodiscard]] std::shared_ptr<FramebufferAttachment> getDepthAttachment() const {
        return m_depth_attachment;
    }
    [[nodiscard]] std::shared_ptr<FramebufferAttachment> getStencilAttachment() const {
        return m_stencil_attachment;
    }
    [[nodiscard]] std::unordered_map<int, std::shared_ptr<FramebufferAttachment>> getColorAttachments() const {
        return m_color_attachments;
    }
    [[nodiscard]] std::shared_ptr<FramebufferAttachment> getColorAttachment(int slot) const {
        return m_color_attachments.at(slot);
    }
    [[nodiscard]] GLuint id() const override { return m_fbo; }
    [[nodiscard]] int width() const override { return m_width; }
    [[nodiscard]] int height() const override { return m_height; }

    Image getColorImage(int slot) const;

    Image getDepthImage() const;

    Image getStencilImage() const;



    void debug() const;


private:
    GLuint m_fbo = 0;
    GLuint depthRbo = 0;
    int m_width = -1;
    int m_height = -1;
    std::unordered_map<int, std::shared_ptr<FramebufferAttachment>> m_color_attachments;
    std::shared_ptr<FramebufferAttachment> m_depth_attachment;
    std::shared_ptr<FramebufferAttachment> m_stencil_attachment;

    Framebuffer();

    void attach(GLenum attachmentPoint, const std::shared_ptr<FramebufferAttachment>& attachment);

    void validate_attachment_size(const std::shared_ptr<FramebufferAttachment> &attachment);

    void invalidate_dimensions_if_empty();
};

#endif //FRAMEBUFFER_H
