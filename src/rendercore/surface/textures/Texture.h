#ifndef TEXTURE_H
#define TEXTURE_H


#include "rendercore/core/enums/textures/FramebufferAttachment.h"
#include "rendercore/opengl/GLContext.h"
#include "rendercore/surface/images/Image.h"



class Texture2D:
    public FramebufferAttachment,
    public std::enable_shared_from_this<Texture2D>
{
public:
    
    static std::shared_ptr<Texture2D> create();

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&&) noexcept = default;
    Texture2D& operator=(Texture2D&&) noexcept = default;
    
    ~Texture2D() override;

    std::shared_ptr<Texture2D> set_generate_mipmap(bool generateMipmap);

    void set_image(const Image& img);

    Image get_image() const;

    void allocate(int w, int h, PixelStorageFormat fmt = PixelStorageFormat::RGBA8, bool is_mutable = true);

    void fill(const Color& color) const;

    void set_border_color(const float* color) const;

    void debug(const std::string &msg) const;

    void update(const Image& img, int x = 0, int y = 0, int w = -1, int h = -1) const;

    void regenerate_mipmap() const;

    bool is_uploaded() const { return m_is_allocated; }

    bool should_generate_mipmap() const { return m_should_generate_mipmap; }

    void bind() const override;

    void bind(int unit) const;

    void resize(int width, int height) override;

    void set_default_sampler(const std::shared_ptr<Sampler>& sampler) { m_default_sampler = sampler; }

    std::shared_ptr<Sampler> get_default_sampler() const { return m_default_sampler; }
    [[nodiscard]] int width() const override { return m_width; }
    [[nodiscard]] int height() const override { return m_height; }
    [[nodiscard]] PixelStorageFormat format() const override { return m_format; }
    [[nodiscard]] GLuint id() const override { return m_id; }
    [[nodiscard]] AttachmentType type() const override { return AttachmentType::Texture; }

protected:
    inline static bool is_initialized = false;


    GLuint m_id = 0;
    bool m_is_allocated = false;
    bool m_is_mutable = false;
    bool m_is_params_dirty = true;
    bool m_should_generate_mipmap = true;



    int m_width = 0;
    int m_height = 0;
    PixelStorageFormat m_format = PixelStorageFormat::RGBA8;

    std::shared_ptr<Sampler> m_default_sampler = nullptr;

    static int mip_levels(int w, int h);

    static void init();

    Texture2D();
};
#endif //TEXTURE_H
