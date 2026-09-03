#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include "Window.h"
#include <windows.h>
#include <cassert>
#include <cstring>

struct Window::Impl {
    MSG msg = {};
    HWND hwnd = nullptr;
    BITMAPINFO bmi = {};
    HBITMAP hBitmap = nullptr;
    HBITMAP oldBitmap = nullptr;
    HDC hdcScreen = nullptr;
    HDC hdcMem = nullptr;
    BLENDFUNCTION blend = {};
};


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_ERASEBKGND:
            return 1;
        case WM_NCCALCSIZE:
            return 0;
        case WM_SETCURSOR:
            SetCursor(LoadCursor(nullptr, IDC_ARROW));
            return TRUE;
        default:
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



std::shared_ptr<Window> Window::create(
    const std::string& title ,
    int w, int h,
    int x,
    int y,
    int glMayorVersion,
    int glMinorVersion,
    bool glDirectDraw,
    bool visible) {
    return std::shared_ptr<Window>(new Window(title, w, h, x, y, glMayorVersion, glMinorVersion, glDirectDraw, visible));
}


Window::~Window() {
    wglMakeCurrent(nullptr, nullptr);
}

std::shared_ptr<Window> Window::set_visible(const bool visible) {
    if (visible)
        ShowWindow(impl->hwnd, SW_SHOW);
    else
        ShowWindow(impl->hwnd, SW_HIDE);
    return shared_from_this();
}

HWND toHWND(const WindowZLayers layer) {
    switch(layer) {
        case WindowZLayers::ALWAYS_ON_TOP: return HWND_TOPMOST;
        case WindowZLayers::TOP:           return HWND_TOP;
        case WindowZLayers::NORMAL:        return HWND_NOTOPMOST;
        case WindowZLayers::BOTTOM:        return HWND_BOTTOM;
    }
    return nullptr;
}

std::shared_ptr<Window> Window::position(int x, int y) {
    switch (x) {
        case WindowPositions::CENTER_X: {
            x = (getMonitorWidth() - w) / 2;
            this->x=x;
            break;
        }
        case WindowPositions::LEFT : {
            x = 0;
            this->x=x;
            break;
        }
        case WindowPositions::RIGHT : {
            x = getMonitorWidth() - w;
            this->x=x;
            break;
        }
        default:
            this->x = x;
    }
    switch (y) {
        case WindowPositions::CENTER_Y: {
            y = (getMonitorHeight() - h) / 2;
            this->y=y;
            break;
        }
        case WindowPositions::TOP : {
            y = 0;
            this->y=0;
            break;
        }
        case WindowPositions::BOTTOM : {
            y = getMonitorHeight() - h;
            this->y=y;
            break;
        }
        default:
            this->y = y;
    }
    SetWindowPos(impl->hwnd, toHWND(layer), x, y, w, h, SWP_NOSIZE);
    return shared_from_this();
}

std::shared_ptr<Window> Window::size(const int width, const int height) {
    this->w = width;
    this->h = height;
    glViewport(0, 0, w, h);
    SetWindowPos(impl->hwnd, toHWND(layer), 0, 0, width, height, SWP_NOMOVE);
    return shared_from_this();
}


[[nodiscard]] PixelBuf Window::getBackBuf() {
    std::vector<GLubyte> temp(w * h * 4);
    auto result = new uint32_t[w * h];
    context->make_current();
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, temp.data());
    for (int i = 0; i < w * h; ++i) {
        result[i] = (temp[i*4+3] << 24) | (temp[i*4] << 16) | (temp[i*4+1] << 8) | temp[i*4+2];
    }
    return {result, w, h};
}

[[nodiscard]] std::shared_ptr<Framebuffer> Window::getFramebuffer() const {
    return fbo;
}

void Window::flipBuffers() {
    while(PeekMessage(&impl->msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&impl->msg);
        DispatchMessage(&impl->msg);
    }
    if (glDirectDraw) {
        context->swap_buffers();
    }
    else {
        bind();
        std::vector<GLubyte> buf(w * h * 4);
        glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buf.data());
        for (int y = 0; y < h; ++y) {
            int flippedY = h - 1 - y;
            for (int x = 0; x < w; ++x) {
                int srcIdx = (y * w + x) * 4;
                uint8_t r = buf[srcIdx];
                uint8_t g = buf[srcIdx + 1];
                uint8_t b = buf[srcIdx + 2];
                uint8_t a = buf[srcIdx + 3];
                r = (r * a + 127) / 255;
                g = (g * a + 127) / 255;
                b = (b * a + 127) / 255;
                backBuf[flippedY * w + x] = (a << 24) | (r << 16) | (g << 8) | b;
            }
        }
        POINT winpos = { x, y };
        SIZE sizeWin = { w, h };
        POINT ptSrc = { 0, 0 };
        std::memcpy(frontBuf, backBuf.data(), backBuf.size() * sizeof(uint32_t));
        UpdateLayeredWindow(impl->hwnd, impl->hdcScreen, &winpos, &sizeWin,
                        impl->hdcMem, &ptSrc, 0, &impl->blend, ULW_ALPHA | SWP_NOMOVE);
    }
}

void Window::bind() const {
    if (glDirectDraw) {
        bind_framebuffer(0);
    }
    else {
        assert(fbo!=nullptr);
        fbo->bind();
    }
}


Window::Window (
    const std::string& title,
    int w, int h,
    int x, int y,
    int glMayorVersion,
    int glMinorVersion,
    bool glDirectDraw,
    bool visible
) : impl(std::make_unique<Impl>()) {
    constexpr char CLASS_NAME[] = "WindowClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = static_cast<HINSTANCE>(getHInstance());
    wc.lpszClassName = CLASS_NAME;
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    static bool registered = false;
    if (!registered) {
        RegisterClass(&wc);
        registered = true;
    }

    if (w < 0)
        w = getMonitorWidth();
    if (h < 0)
        h = getMonitorHeight();
    switch (x) {
        case WindowPositions::CENTER_X: {
            x = (getMonitorWidth() - w) / 2;
            this->x = x;
            break;
        }
        case WindowPositions::LEFT: {
            x = 0;
            this->x = x;
            break;
        }
        case WindowPositions::RIGHT: {
            x = getMonitorWidth() - w;
            this->x = x;
            break;
        }
        default:
            this->x = x;
    }
    switch (y) {
        case WindowPositions::CENTER_Y: {
            y = (getMonitorHeight() - h) / 2;
            this->y = y;
            break;
        }
        case WindowPositions::TOP: {
            y = 0;
            this->y = 0;
            break;
        }
        case WindowPositions::BOTTOM: {
            y = getMonitorHeight() - h;
            this->y = y;
            break;
        }
        default:
            this->y = y;
    }

    this->w = w;
    this->h = h;

    impl->hdcScreen = GetDC(nullptr);

    DWORD exStyle = WS_EX_LAYERED;

    if (glDirectDraw)
        exStyle = 0;
    impl->hwnd = CreateWindowEx(
        exStyle,
        CLASS_NAME,
        title.c_str(),
        WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT, w, h,
        nullptr, nullptr, static_cast<HINSTANCE>(getHInstance()), nullptr
    );

    context = std::make_unique<GLContext>(impl->hwnd, glMayorVersion, glMinorVersion);
    glViewport(0, 0, w, h);
    this->glDirectDraw = glDirectDraw;
    if (!glDirectDraw) {
        createFBO();
        Window::bind();
        setUpCPUBuffer();
    }
    SetWindowPos(impl->hwnd, HWND_TOP, x, y, w, h, SWP_NOSIZE);

    setUpCPUBuffer();
    if (visible)
        ShowWindow(impl->hwnd, SW_SHOW);
}

void Window::createFBO() {
    fbo = Framebuffer::create();
    fbo->bind();
    texture = Texture2D::create();
    texture->allocate(w, h, PixelStorageFormat::RGBA8);
    fbo->add_color_attachment(texture, 0);
    GLContext::current_context->state.current_framebuffer = fbo->id();
}


void Window::resize_buffers(const int new_w, const int new_h) {
    std::vector<uint32_t> newBuf(new_w * new_h, 0);

    const int copy_w = std::min(w, new_w);
    const int copy_h = std::min(h, new_h);

    for (int y = 0; y < copy_h; ++y) {
        std::memcpy(
            newBuf.data() + y * new_w,
            backBuf.data() + y * w,
            copy_w * sizeof(uint32_t)
        );
    }
    if (impl->hBitmap) {
        SelectObject(impl->hdcMem, impl->oldBitmap);
        DeleteObject(impl->hBitmap);
        impl->hBitmap = nullptr;
        frontBuf = nullptr;
    }

    impl->bmi.bmiHeader.biWidth  = new_w;
    impl->bmi.bmiHeader.biHeight = -new_h;

    fbo->resize(new_w, new_h);

    void* bits = nullptr;
    HBITMAP newBitmap = CreateDIBSection(
    impl->hdcMem,
    &impl->bmi,
    DIB_RGB_COLORS,
    &bits,
    nullptr,
    0
    );
    if (!newBitmap || !bits) {
        return;
    }
    impl->hBitmap = newBitmap;
    impl->oldBitmap = static_cast<HBITMAP>(SelectObject(impl->hdcMem, impl->hBitmap));
    frontBuf = static_cast<uint32_t*>(bits);
    backBuf.swap(newBuf);
    std::memcpy(frontBuf, backBuf.data(), backBuf.size() * sizeof(uint32_t));
}

void Window::setUpCPUBuffer() {
    impl->hdcMem = CreateCompatibleDC(impl->hdcScreen);

    impl->bmi = {};
    impl->bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    impl->bmi.bmiHeader.biWidth = w;
    impl->bmi.bmiHeader.biHeight = -h;
    impl->bmi.bmiHeader.biPlanes = 1;
    impl->bmi.bmiHeader.biBitCount = 32;
    impl->bmi.bmiHeader.biCompression = BI_RGB;
    void* pvBits = nullptr;
    impl->hBitmap = CreateDIBSection(impl->hdcMem, &impl->bmi, DIB_RGB_COLORS, &pvBits, nullptr, 0);
    SelectObject(impl->hdcMem, impl->hBitmap);
    frontBuf = static_cast<uint32_t *>(pvBits);
    for(int py=0; py<h; ++py){
        for(int px=0; px<w; ++px){
            frontBuf[py*w + px] = 0xffffffff;
        }
    }
    POINT ptWinPos = { x, y };
    SIZE sizeWin = { w, h };
    POINT ptSrc = { 0, 0 };

    impl->blend = {};
    impl->blend.BlendOp = AC_SRC_OVER;
    impl->blend.SourceConstantAlpha = 255;
    impl->blend.AlphaFormat = AC_SRC_ALPHA;
    impl->blend.BlendFlags = 0;
    UpdateLayeredWindow(impl->hwnd, impl->hdcScreen, &ptWinPos, &sizeWin,
                        impl->hdcMem, &ptSrc, 0, &impl->blend, ULW_ALPHA);
    backBuf = std::vector<uint32_t>(w*h, 0);
    for(int py=0; py<h; ++py){
        for(int px=0; px<w; ++px){
            backBuf[py*w + px] = 0x44020424;
        }
    }
}





#endif // _WIN32

