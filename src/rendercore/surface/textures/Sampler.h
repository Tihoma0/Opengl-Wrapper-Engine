#ifndef SAMPLER_H
#define SAMPLER_H
// #define CACHE_SAMPLER_PARAMS

#include <iostream>
#include <memory>


#include "glad/glad.h"
#include "rendercore/opengl/GLTrack.h"


enum class Filter {
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear
};

enum class WrapMode {
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder,
    MirrorClampToEdge
};

[[nodiscard]] constexpr GLint to_gl(Filter filter);

[[nodiscard]] constexpr GLint to_gl(WrapMode mode);


class Sampler : public std::enable_shared_from_this<Sampler> {
public:
    
    static std::shared_ptr<Sampler> create();


    ~Sampler() {
        glDeleteSamplers(1, &m_id);
    }



    std::shared_ptr<Sampler> set_filters(Filter min, Filter mag);


    std::shared_ptr<Sampler> set_wrap_mode(WrapMode wrap_s, WrapMode wrap_t, WrapMode wrap_r);


    std::shared_ptr<Sampler> set_wrap_mode(WrapMode wrap);


    std::shared_ptr<Sampler> set_min_lod(float v);

    std::shared_ptr<Sampler> set_max_lod(float v);

    std::shared_ptr<Sampler> set_lod(float min, float max);

    std::shared_ptr<Sampler> set_lod_bias(float v);

    std::shared_ptr<Sampler> set_lod(float min, float max, float bias);

    std::shared_ptr<Sampler> set_border_color(float r, float g, float b, float a);

    void bind() const {
        bind(0);
    }

    void bind(const int slot) const {
        bind_sampler(slot, m_id);
    }

#ifdef CACHE_SAMPLER_PARAMS
    [[nodiscard]] Filter get_min_filter() const { return minFilter; }
    [[nodiscard]] Filter get_mag_filter() const { return magFilter; }
    [[nodiscard]] WrapMode get_wrap_s() const { return wrapS; }
    [[nodiscard]] WrapMode get_wrap_t() const { return wrapT; }
    [[nodiscard]] WrapMode get_wrap_r() const { return wrapR; }

    [[nodiscard]] GLfloat get_min_lod() const { return minLod; }
    [[nodiscard]] GLfloat get_max_lod() const { return maxLod; }
    [[nodiscard]] GLfloat get_lod_bias() const { return lodBias; }

    [[nodiscard]] GLfloat get_border_color_r() const { return borderColor[0]; }
    [[nodiscard]] GLfloat get_border_color_g() const { return borderColor[1]; }
    [[nodiscard]] GLfloat get_border_color_b() const { return borderColor[2]; }
    [[nodiscard]] GLfloat get_border_color_a() const { return borderColor[3]; }
    [[nodiscard]] GLfloat* get_border_color() { return borderColor; }
    void print_filters() {
        std::string min_text = "min filter: ";
        switch (get_min_filter()) {
            case Filter::Nearest:
                min_text += "Nearest";
                break;
            case Filter::Linear:
                min_text += "Linear";
                break;
            case Filter::NearestMipmapNearest:
                min_text += "NearestMipmapNearest";
                break;
            case Filter::LinearMipmapNearest:
                min_text += "LinearMipmapNearest";
                break;
            case Filter::NearestMipmapLinear:
                min_text += "NearestMipmapLinear";
                break;
            case Filter::LinearMipmapLinear:
                min_text += "LinearMipmapLinear";
                break;
        }

        std::string mag_text = "mag filter: ";
        switch (get_mag_filter()) {
            case Filter::Nearest:
                mag_text += "Nearest";
                break;
            case Filter::Linear:
                mag_text += "Linear";
                break;
        }

        std::cout << min_text << '\n';
        std::cout << mag_text << '\n';
        GLint filter;

        glGetSamplerParameteriv(
            m_id,
            GL_TEXTURE_MIN_FILTER,
            &filter
        );

        std::cout << "min filter: 0x"
                  << std::hex << filter
                  << std::dec << '\n';
    }
#endif // CACHE_SAMPLER_PARAMS



private:

    GLuint m_id = 0;
    Sampler();
#ifdef CACHE_SAMPLER_PARAMS
    Filter minFilter = Filter::Nearest;
    Filter magFilter = Filter::Nearest;
    WrapMode wrapS = WrapMode::Repeat;
    WrapMode wrapT = WrapMode::Repeat;
    WrapMode wrapR = WrapMode::Repeat;


    GLfloat minLod = -1000.0f;
    GLfloat maxLod = 1000.0f;
    GLfloat lodBias = 0.0f;

    GLfloat borderColor[4] = {0,0,0,0};
#endif // CACHE_SAMPLER_PARAMS
};
#endif //SAMPLER_H
