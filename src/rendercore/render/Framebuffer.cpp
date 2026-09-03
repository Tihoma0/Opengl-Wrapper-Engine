#include "rendercore/render/Framebuffer.h"

#include <iostream>
#include <memory>
#include <unordered_map>

#include "rendercore/core/enums/textures/FramebufferAttachment.h"
#include "glad/glad.h"
#include "rendercore/core/Warnings.h"
#include "rendercore/render/Renderbuffer.h"
#include "rendercore/surface/textures/Texture.h"



void Framebuffer::resize(const int width, const int height) {
    m_width = width;
    m_height = height;
    for (auto& [slot, attachment] : m_color_attachments)
        attachment->resize(width, height);
    if (m_depth_attachment)
        m_depth_attachment->resize(width, height);
    if (m_stencil_attachment && m_stencil_attachment != m_depth_attachment)
        m_stencil_attachment->resize(width, height);
    bind();
}


void Framebuffer::add_color_attachment(const std::shared_ptr<FramebufferAttachment> &attachment, int slot) {
    validate_attachment_size(attachment);
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, attachment->id(), 0);
    m_color_attachments[slot] = attachment;
    std::vector<GLenum> keys;
    keys.reserve(m_color_attachments.size());

    for (const auto& [key, value] : m_color_attachments) {
        keys.push_back(GL_COLOR_ATTACHMENT0 + key);
    }
    glDrawBuffers(keys.size(), keys.data());
}

void Framebuffer::add_depth_attachment(const std::shared_ptr<FramebufferAttachment> &attachment) {
    if (m_depth_attachment)
        THROW_RUNTIME("Depth attachment already exists");
    if (get_role(attachment->format()) != FormatRole::Depth)
        THROW_RUNTIME("Depth attachment must be a depth buffer");
    validate_attachment_size(attachment);
    bind();
    attach(GL_DEPTH_ATTACHMENT, attachment);
    m_depth_attachment = attachment;
}

void Framebuffer::add_stencil_attachment(const std::shared_ptr<FramebufferAttachment> &attachment) {
    if (m_stencil_attachment)
        THROW_RUNTIME("Stencil attachment already exists");
    if (get_role(attachment->format()) != FormatRole::Stencil)
        THROW_RUNTIME("Stencil attachment must be a stencil buffer");
    validate_attachment_size(attachment);
    bind();
    attach(GL_STENCIL_ATTACHMENT, attachment);
    m_stencil_attachment = attachment;
}

void Framebuffer::add_depth_stencil_attachment(const std::shared_ptr<FramebufferAttachment> &attachment) {
    if (m_depth_attachment)
        THROW_RUNTIME("Depth attachment already exists");
    if (m_stencil_attachment)
        THROW_RUNTIME("Stencil attachment already exists");
    if (get_role(attachment->format()) != FormatRole::DepthStencil)
        THROW_RUNTIME("Depth stencil attachment must be a depth stencil buffer");
    validate_attachment_size(attachment);
    bind();
    attach(GL_DEPTH_STENCIL_ATTACHMENT, attachment);
    m_depth_attachment = attachment;
    m_stencil_attachment = attachment;
}

std::shared_ptr<FramebufferAttachment> Framebuffer::detach_depth_attachment() {
    if (m_stencil_attachment == m_depth_attachment)
        THROW_RUNTIME("A Depth-Stencil attachment cannot be detached via detach_depth_attachment()");
    std::shared_ptr<FramebufferAttachment> attachment = m_depth_attachment;
    m_depth_attachment = nullptr;
    invalidate_dimensions_if_empty();
    return attachment;
}

std::shared_ptr<FramebufferAttachment> Framebuffer::detach_stencil_attachment() {
    if (m_stencil_attachment == m_depth_attachment)
        THROW_RUNTIME("A Depth-Stencil attachment cannot be detached via detach_stencil_attachment()");
    std::shared_ptr<FramebufferAttachment> attachment = m_stencil_attachment;
    m_stencil_attachment = nullptr;
    invalidate_dimensions_if_empty();
    return attachment;
}

std::shared_ptr<FramebufferAttachment> Framebuffer::detach_depth_stencil_attachment() {
    if (m_stencil_attachment != m_depth_attachment)
        THROW_RUNTIME("No Depth-Stencil attachment exists");
    std::shared_ptr<FramebufferAttachment> attachment = m_depth_attachment;
    m_depth_attachment = nullptr;
    m_stencil_attachment = nullptr;
    invalidate_dimensions_if_empty();
    return attachment;
}

std::shared_ptr<FramebufferAttachment> Framebuffer::detach_color_attachment(int slot) {
    std::shared_ptr<FramebufferAttachment> attachment = m_color_attachments[slot];
    m_color_attachments.erase(slot);
    invalidate_dimensions_if_empty();
    return attachment;
}


Image Framebuffer::getColorImage(int slot) const {
    if (m_color_attachments.find(slot) == m_color_attachments.end()) {
        THROW_RUNTIME("No color attachment exists at slot " + std::to_string(slot));
    }
    bind();
    const PixelStorageFormat fmt = m_color_attachments.at(slot)->format();
    auto result = Image(m_width, m_height, fmt);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + slot);
    glReadPixels(
        0,
        0,
        width(),
        height(),
        to_gl_external_format(fmt),
        to_gl_type(fmt),
        result.buffer().data()
    );
    return result;
}

Image Framebuffer::getDepthImage() const {
    if (!m_depth_attachment) {
        THROW_RUNTIME("Framebuffer has no attached depth buffer.");
    }
    bind();
    glReadBuffer(GL_NONE);
    auto result = Image(m_width, m_height, PixelStorageFormat::DEPTH32F);  // PixelStorageFormat::DEPTH32F is what opengl converts all depth formats to when reading back
    glReadPixels(0, 0, width(), height(), GL_DEPTH_COMPONENT, GL_FLOAT, result.buffer().data());
    result.set_type(DepthImage);
    return result;
}

Image Framebuffer::getStencilImage() const {
    if (!m_stencil_attachment) {
        THROW_RUNTIME("Framebuffer has no attached stencil buffer.");
    }
    bind();
    glReadBuffer(GL_NONE);
    auto result = Image(m_width, m_height, PixelStorageFormat::STENCIL8);
    glReadPixels(0, 0, width(), height(), GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, result.buffer().data());
    result.set_type(StencilImage);
    return result;
}

void Framebuffer::debug() const {
    bind();

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    std::cout << "FBO status: 0x" << std::hex << status << std::endl;

    GLint type = 0;
    GLint name = 0;

    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
        &type
    );

    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
        &name
    );



    std::cout << "COLOR_ATTACHMENT0 type: ";
    if (type == GL_TEXTURE) std::cout << "GL_TEXTURE";
    else if (type == GL_RENDERBUFFER) std::cout << "GL_RENDERBUFFER";
    else if (type == GL_NONE) std::cout << "GL_NONE";
    else std::cout << "UNKNOWN";

    std::cout << ", id: " << name << std::endl;
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
        &type
    );

    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
        &name
    );

    std::cout << "DEPTH_STENCIL_ATTACHMENT type: ";
    if (type == GL_TEXTURE) std::cout << "GL_TEXTURE";
    else if (type == GL_RENDERBUFFER) std::cout << "GL_RENDERBUFFER";
    else if (type == GL_NONE) std::cout << "GL_NONE";
    else std::cout << "UNKNOWN";

    std::cout << ", id: " << name << std::endl;
}



Framebuffer::Framebuffer() {
    m_fbo = 0;
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    GLContext::current_context->state.current_framebuffer = m_fbo;
}

void Framebuffer::attach(GLenum attachmentPoint,
        const std::shared_ptr<FramebufferAttachment>& attachment)
{
    if (attachment->type() == AttachmentType::Renderbuffer)
    {
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER,
            attachmentPoint,
            GL_RENDERBUFFER,
            attachment->id()
        );
    }
    else if (attachment->type() == AttachmentType::Texture)
    {
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            attachmentPoint,
            GL_TEXTURE_2D,
            attachment->id(),
            0
        );
    }
    else
    {
        THROW_RUNTIME("Unsupported attachment type");
    }
}

void Framebuffer::validate_attachment_size(const std::shared_ptr<FramebufferAttachment> &attachment) {
    if (attachment->width() <= 0 || attachment->height() <= 0)
        THROW_RUNTIME("Invalid attachment dimensions");
    if (m_width == -1 && m_height == -1) {
        m_width = attachment->width();
        m_height = attachment->height();
    }
    if (m_width != attachment->width() || m_height != attachment->height())
        THROW_RUNTIME("All attachments must have the same dimensions");
}

void Framebuffer::invalidate_dimensions_if_empty() {
    if (!m_stencil_attachment && !m_depth_attachment && m_color_attachments.empty()) {
        m_width = -1;
        m_height = -1;
    }
}


