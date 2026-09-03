#ifndef WINDOW_H

#define WINDOW_H

#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif

#define WIN32_LEAN_AND_MEAN
#include <memory>

#include "WindowUtils.h"
#include "../render/RenderTarget.h"
#include "../render/Framebuffer.h"


class Window:
std::enable_shared_from_this<Window>,
public RenderTarget
{

public:

    static std::shared_ptr<Window> create(
        const std::string& title = "QWindow",
        int w = -1, int h = -1,
        int x = WindowPositions::CENTER_X,
        int y = WindowPositions::CENTER_Y,
        int glMayorVersion = 4,
        int glMinorVersion = 5,
        bool glDirectDraw = true,
        bool visible = true);


    ~Window() override;

    std::shared_ptr<Window> set_visible(bool visible);

    std::shared_ptr<Window> position(int x, int y);

    std::shared_ptr<Window> size(int width, int height);

    [[nodiscard]] int getH() const { return h; }
    [[nodiscard]] int getW() const { return w; }

    [[nodiscard]] PixelBuf getBackBuf();

    std::shared_ptr<Framebuffer> getFramebuffer() const;

    void flipBuffers();
    GLContext& getContext() const { return *context; }

    void make_current() const {
        context->make_current();
    }

    void bind() const override;

    [[nodiscard]] GLuint id() const override { return fbo->id(); }
    [[nodiscard]] int width() const override { return w; }
    [[nodiscard]] int height() const override { return h; }

private:
    struct Impl;
    std::unique_ptr<Impl> impl;

    uint32_t* frontBuf = nullptr;
    std::vector<uint32_t> backBuf;
    WindowZLayers layer = WindowZLayers::NORMAL;
    std::unique_ptr<GLContext> context;
    int x, y;
    int w = 1500;
    int h = 700;
    std::shared_ptr<Framebuffer> fbo;
    std::shared_ptr<Texture2D> texture;
    bool glDirectDraw = false;

    Window(
        const std::string& title,
        int w, int h,
        int x, int y,
        int glMayorVersion,
        int glMinorVersion,
        bool glDirectDraw,
        bool visible
    );

    void createFBO();


    void resize_buffers(int new_w, int new_h);
    void setUpCPUBuffer();
    static constexpr char CLASS_NAME[] = "WindowClass";


};



class WindowBuilder {
public:
    std::string m_title = "QWindow";
    int m_width = -1;
    int m_height = -1;
    int m_x = WindowPositions::CENTER_X;
    int m_y = WindowPositions::CENTER_Y;
    int m_glMayorVersion = 4;
    int m_glMinorVersion = 5;
    bool m_glDirectDraw = true;
    bool m_visible = true;

    WindowBuilder() = default;

    WindowBuilder& title(std::string title) {
        this->m_title = std::move(title);
        return *this;
    }

    WindowBuilder& size(int width, int height) {
        this->m_width = width;
        this->m_height = height;
        return *this;
    }

    WindowBuilder& position(int x, int y) {
        this->m_x= x;
        this->m_y = y;
        return *this;
    }

    WindowBuilder& glDirectDraw(bool is_direct) {
        m_glDirectDraw = is_direct;
        return *this;
    }

    WindowBuilder& visible(bool visible) {
        this->m_visible = visible;
        return *this;
    }



    [[nodiscard]] std::shared_ptr<Window> build() const {
        return Window::create(m_title, m_width, m_height, m_x, m_y, m_glMayorVersion, m_glMinorVersion, m_glDirectDraw, m_visible);
    }
};

#endif // _WIN32


#endif //WINDOW_H
