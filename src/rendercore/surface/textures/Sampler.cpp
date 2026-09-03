#include "Sampler.h"
#include <memory>
#include "glad/glad.h"


[[nodiscard]] constexpr GLint to_gl(const Filter filter)
{
    switch (filter)
    {
        case Filter::Nearest:                return GL_NEAREST;
        case Filter::Linear:                 return GL_LINEAR;
        case Filter::NearestMipmapNearest:   return GL_NEAREST_MIPMAP_NEAREST;
        case Filter::LinearMipmapNearest:    return GL_LINEAR_MIPMAP_NEAREST;
        case Filter::NearestMipmapLinear:    return GL_NEAREST_MIPMAP_LINEAR;
        case Filter::LinearMipmapLinear:     return GL_LINEAR_MIPMAP_LINEAR;
    }

    return 0;
}

[[nodiscard]] constexpr GLint to_gl(const WrapMode mode)
{
    switch (mode)
    {
        case WrapMode::Repeat:           return GL_REPEAT;
        case WrapMode::MirroredRepeat:   return GL_MIRRORED_REPEAT;
        case WrapMode::ClampToEdge:      return GL_CLAMP_TO_EDGE;
        case WrapMode::ClampToBorder:    return GL_CLAMP_TO_BORDER;
        case WrapMode::MirrorClampToEdge:return GL_MIRROR_CLAMP_TO_EDGE;
    }
    return 0;
}



std::shared_ptr<Sampler> Sampler::create() {
    struct EnableMakeShared : Sampler {
        EnableMakeShared() : Sampler() {}
    };
    return std::make_shared<EnableMakeShared>();
}


std::shared_ptr<Sampler> Sampler::set_filters(Filter min, Filter mag) {
    glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, to_gl(min));
    glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, to_gl(mag));
#ifdef CACHE_SAMPLER_PARAMS
    minFilter = min;
    magFilter = mag;
#endif // CACHE_SAMPLER_PARAMS
    return shared_from_this();
}


std::shared_ptr<Sampler> Sampler::set_wrap_mode(WrapMode wrap_s, WrapMode wrap_t, WrapMode wrap_r) {
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, to_gl(wrap_s));
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, to_gl(wrap_t));
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_R, to_gl(wrap_r));

#ifdef CACHE_SAMPLER_PARAMS
    wrapS = wrap_s;
    wrapT = wrap_t;
    wrapR = wrap_r;
#endif // CACHE_SAMPLER_PARAMS
    return shared_from_this();
}


std::shared_ptr<Sampler> Sampler::set_wrap_mode(WrapMode wrap) {
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, to_gl(wrap));
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, to_gl(wrap));
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_R, to_gl(wrap));

#ifdef CACHE_SAMPLER_PARAMS
    wrapS = wrap;
    wrapT = wrap;
    wrapR = wrap;
#endif // CACHE_SAMPLER_PARAMS
    return shared_from_this();
}


std::shared_ptr<Sampler> Sampler::set_min_lod(const float v) {
    glSamplerParameterf(m_id, GL_TEXTURE_MIN_LOD, v);
#ifdef CACHE_SAMPLER_PARAMS
    minLod = v;
#endif // CACHE_SAMPLER_PARAMS
    return shared_from_this();
}

std::shared_ptr<Sampler> Sampler::set_max_lod(const float v) {
    glSamplerParameterf(m_id, GL_TEXTURE_MAX_LOD, v);
#ifdef CACHE_SAMPLER_PARAMS
    maxLod = v;
#endif // CACHE_SAMPLER_PARAMS
    return shared_from_this();
}

std::shared_ptr<Sampler> Sampler::set_lod(const float min, const float max) {
    set_min_lod(min);
    set_max_lod(max);
    return shared_from_this();
}

std::shared_ptr<Sampler> Sampler::set_lod_bias(const float v) {
    glSamplerParameterf(m_id, GL_TEXTURE_LOD_BIAS, v);
#ifdef CACHE_SAMPLER_PARAMS
    lodBias = v;
#endif // CACHE_SAMPLER_PARAMS
    return shared_from_this();
}

std::shared_ptr<Sampler> Sampler::set_lod(float min, float max, float bias)
{
    set_min_lod(min);
    set_max_lod(max);
    set_lod_bias(bias);
    return shared_from_this();
}


std::shared_ptr<Sampler> Sampler::set_border_color(const float r, const float g, const float b, const float a)
{
    const GLfloat color[4] = { r, g, b, a };
    glSamplerParameterfv(m_id, GL_TEXTURE_BORDER_COLOR, color);
    return shared_from_this();
}

#ifdef CACHE_SAMPLER_PARAMS
Sampler::Sampler()
{
    glGenSamplers(1, &m_id);

    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, to_gl(wrapS));
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, to_gl(wrapT));
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_R, to_gl(wrapR));
    glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, to_gl(minFilter));
    glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, to_gl(magFilter));
    glSamplerParameterf(m_id, GL_TEXTURE_MIN_LOD, minLod);
    glSamplerParameterf(m_id, GL_TEXTURE_MAX_LOD, maxLod);
    glSamplerParameterf(m_id, GL_TEXTURE_LOD_BIAS, lodBias);
    glSamplerParameterfv(m_id, GL_TEXTURE_BORDER_COLOR, borderColor);
}
#else
Sampler::Sampler()
{
    glGenSamplers(1, &m_id);

    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_R, GL_NEAREST);
    glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameterf(m_id, GL_TEXTURE_MIN_LOD, -1000);
    glSamplerParameterf(m_id, GL_TEXTURE_MAX_LOD, 1000);
    glSamplerParameterf(m_id, GL_TEXTURE_LOD_BIAS, 0);
    constexpr GLfloat borderColor[4] = { 0, 0, 0, 0 };
    glSamplerParameterfv(m_id, GL_TEXTURE_BORDER_COLOR, borderColor);
}
#endif // CACHE_SAMPLER_PARAMS