#include "Texture.h"

#include <cmath>
#include <iostream>

#include "rendercore/surface/images/Image.h"
#include "rendercore/surface/Color.h"
#include "Sampler.h"
#include "glad/glad.h"
#include "rendercore/core/Warnings.h"
#include "rendercore/core/enums/textures/FramebufferAttachment.h"
#include "rendercore/opengl/GLContext.h"
#include "rendercore/opengl/GLTrack.h"



std::shared_ptr<Texture2D> Texture2D::create() {
    return std::shared_ptr<Texture2D>(new Texture2D());
}



Texture2D::~Texture2D() {
    glDeleteTextures(1, &m_id);
}

std::shared_ptr<Texture2D> Texture2D::set_generate_mipmap(bool generateMipmap) {
    m_should_generate_mipmap = generateMipmap;
    return shared_from_this();
}

void Texture2D::set_image(const Image& img) {
    if (img.width() > GLContext::current_context->limits.maxTextureDimensions || img.height() > GLContext::current_context->limits.maxTextureDimensions)
        THROW_RUNTIME("Texture size too large");
    bind();
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        to_gl_internal_format(img.format()),
        img.width(),
        img.height(),
        0,
        to_gl_external_format(img.format()),
        to_gl_type(img.format()),
        img.buffer().data()
    );
    GLenum error = glGetError();

    if (error == GL_OUT_OF_MEMORY) {
        THROW_RUNTIME("Out of memory");
    }
    if (m_should_generate_mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);
    m_is_allocated = true;
    m_is_mutable = true;
    m_width = img.width();
    m_height = img.height();
    m_format = img.format();
}

Image Texture2D::get_image() const {
    bind();
    auto result = Image(m_width, m_height, m_format);
    glGetTexImage(GL_TEXTURE_2D, 0, to_gl_external_format(m_format), to_gl_type(m_format), result.buffer().data());
    return result;
}

void Texture2D::allocate(const int w, const int h, const PixelStorageFormat fmt, const bool is_mutable) {
    if (w > GLContext::current_context->limits.maxTextureDimensions || h > GLContext::current_context->limits.maxTextureDimensions)
        THROW_RUNTIME("Texture size too large");
    if (GLContext::current_context->supports_dsa()) {
        bind();
        if (is_mutable)
            glTexImage2D(GL_TEXTURE_2D, 0, to_gl_internal_format(fmt), w, h, 0, to_gl_external_format(fmt), to_gl_type(fmt), nullptr);
        else {
            const int mip_levels = m_should_generate_mipmap ? Texture2D::mip_levels(w, h) : 1;
            glTexStorage2D(GL_TEXTURE_2D, mip_levels, to_gl_internal_format(fmt), w, h);
        }
    }
    m_is_allocated = true;
    m_width = w;
    m_height = h;
    m_format = fmt;
    m_is_mutable = is_mutable;
}

void Texture2D::fill(const Color& color) const {
    bind();
    const std::vector<uint8_t> value = color.pack(m_format);
    glClearTexImage(
        m_id,
        0,
        to_gl_external_format(m_format),
        to_gl_type(m_format),
        value.data()
    );
    const GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        std::cout << "fill error: " << err << std::endl;
    if (m_should_generate_mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::set_border_color(const float* color) const {
    bind();
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
}

void Texture2D::debug(const std::string &msg) const {
    std::cout << "debugging texture: " << msg << " is allocated: " << m_is_allocated << " id: " << m_id << " dirty params: " << m_is_params_dirty << " is mutable: " << m_is_mutable << "size: " << m_width << "x" << m_height << std::endl;
}

void Texture2D::update(const Image& img, int x, int y, int w, int h) const {
    if (!subimage_compatible(m_format, img.format()))
        THROW_RUNTIME("Invalid image format");
    if (w == -1)
        w = img.width();
    if (h == -1)
        h = img.height();
    if (x + w > m_width || y + h > m_height)
        THROW_RUNTIME("Subimage update out of bounds");
    bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, w, h, to_gl_external_format(img.format()), GL_UNSIGNED_BYTE, img.buffer().data());
    if (m_should_generate_mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::regenerate_mipmap() const {
    if (!m_is_allocated)
        THROW_RUNTIME("Texture is not allocated");
    bind();
    glGenerateMipmap(GL_TEXTURE_2D);
}


void Texture2D::bind() const {
    bind(0);
}

void Texture2D::bind(const int unit) const {
    bind_texture(unit, m_id);
}

void Texture2D::resize(const int width, const int height) {
    if (width == m_width && height == m_height)
        return;
    allocate(width, height, m_format, m_is_mutable);
}


GLuint m_id = 0;
bool m_is_allocated = false;
bool m_is_mutable = false;
bool m_is_params_dirty = true;
bool m_should_generate_mipmap = true;

int m_width = 0;
int m_height = 0;
PixelStorageFormat m_format = PixelStorageFormat::RGBA8;

std::shared_ptr<Sampler> m_default_sampler = nullptr;

int Texture2D::mip_levels(const int w, const int h) {
    const int max = std::max(w, h);
    return std::log2(max) + 1;
}

void Texture2D::init() {
    if (GLContext::current_context->state.default_sampler)
        return;
    GLContext::current_context->state.default_sampler = Sampler::create();
}

Texture2D::Texture2D() {
    if (!GLContext::current_context) {
        THROW_RUNTIME("GLContext not initialized");
    }
    init();
    glGenTextures(1, &m_id);
    m_default_sampler = GLContext::current_context->state.default_sampler;
}

