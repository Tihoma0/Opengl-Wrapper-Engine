#include "rendercore/opengl/GLContext.h"

#include <cassert>

#include "rendercore/core/Warnings.h"
#include "rendercore/window/WindowUtils.h"

class Sampler;
class GLContext;
class Texture2D;
class Mesh;
class Shader;

#include <windows.h>
#include <GL/wglext.h>


class GLBootstrap{
public:
    static void init();

    static PFNWGLCREATECONTEXTATTRIBSARBPROC getProc();

    static void shutdown();
private:
    static constexpr char CLASS_NAME[] = "GLBootstrapClass";
    inline static bool is_init = false;
    inline static PIXELFORMATDESCRIPTOR pfd_dummy = {};
    inline static HWND dummy = nullptr;
    inline static HDC hdc_dummy = nullptr;
    inline static int pf_dummy = 0;
    inline static HGLRC dummyRC = nullptr;
    inline static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
};

void GLBootstrap::init() {
    if (is_init)
        return;
    dummy = CreateWindowEx(
            0,
            CLASS_NAME,
            "",
            WS_POPUP,
            0, 0, 1, 1,
            nullptr, nullptr, static_cast<HINSTANCE>(getHInstance()), nullptr
        );
    pfd_dummy = {};
    pfd_dummy.nSize = sizeof(pfd_dummy);
    pfd_dummy.nVersion = 1;
    pfd_dummy.dwFlags =
        PFD_DRAW_TO_WINDOW |
        PFD_SUPPORT_OPENGL |
        PFD_DOUBLEBUFFER;
    pfd_dummy.iPixelType = PFD_TYPE_RGBA;
    pfd_dummy.cColorBits = 32;
    pfd_dummy.cDepthBits = 24;
    pfd_dummy.cStencilBits = 8;
    pfd_dummy.iLayerType = PFD_MAIN_PLANE;

    hdc_dummy = GetDC(dummy);

    pf_dummy = ChoosePixelFormat(hdc_dummy, &pfd_dummy);
    const BOOL ok = SetPixelFormat(hdc_dummy, pf_dummy, &pfd_dummy);
    assert(ok);
    dummyRC = wglCreateContext(hdc_dummy);
    wglMakeCurrent(hdc_dummy, dummyRC);

    typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC)(
        HDC, HGLRC, const int*
    );
    wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
    if (!gladLoadGL()) {
    }
    is_init = true;
}

PFNWGLCREATECONTEXTATTRIBSARBPROC GLBootstrap::getProc() {
    if (!is_init)
        init();
    if (!wglCreateContextAttribsARB) {
        THROW_RUNTIME("wglCreateContextAttribsARB not available");
    }
    return wglCreateContextAttribsARB;
}

void GLBootstrap::shutdown() {
    if (!is_init)
        THROW_RUNTIME("GLBootstrap was never initialized");
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(dummyRC);
    ReleaseDC(dummy, hdc_dummy);
    DestroyWindow(dummy);
    is_init = false;
}

GLContext::GLContext(void* hwnd, const int glMayorVersion, const int glMinorVersion, const GLContext* shared_context) {
    const PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = GLBootstrap::getProc();
    m_hdc = GetDC(static_cast<HWND>(hwnd));
    const int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, glMayorVersion,
        WGL_CONTEXT_MINOR_VERSION_ARB, glMinorVersion,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    assert(static_cast<HDC>(m_hdc));
    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pf = ChoosePixelFormat(static_cast<HDC>(m_hdc), &pfd);
    assert(pf);
    SetPixelFormat(static_cast<HDC>(m_hdc), pf, &pfd);
    m_glrc = wglCreateContextAttribsARB(static_cast<HDC>(m_hdc), nullptr, attribs);
    assert(static_cast<HGLRC>(m_glrc));
    if (shared_context) {
        wglShareLists(static_cast<HGLRC>(shared_context->m_glrc), static_cast<HGLRC>(m_glrc));
    }
    wglMakeCurrent(static_cast<HDC>(m_hdc), static_cast<HGLRC>(m_glrc));
    limits = query_limits();
    state.bound_textures.resize(GLState::maxTextureUnits);
    state.bound_samplers.resize(GLState::maxTextureUnits);
    mayor_version = glMayorVersion;
    minor_version = glMinorVersion;
    supports_DSA = at_least_version(4, 5);
    current_context = this;
    blend_state = {};
    blend_state.apply();
}


[[nodiscard]] bool GLContext::at_least_version(int major, int minor) const {
    return this->mayor_version > major ||
           (this->mayor_version == major && this->minor_version >= minor);
}

[[nodiscard]] GLLimits GLContext::query_limits() {
    GLLimits l{};

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &l.maxTextureDimensions);
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &l.max3DTextureSize);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &l.maxTextureUnits);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &l.maxCombinedTextureUnits);
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &l.maxColorAttachments);
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &l.maxDrawBuffers);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &l.maxVertexUniforms);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &l.maxFragmentUniforms);
    GLState::maxTextureUnits = l.maxTextureUnits;
    return l;
}

void GLContext::invalidate_state() {
    state = {};
}

void GLContext::validate_state() const {
    GLint shader = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shader);
    if (state.current_shader != shader)
        THROW_RUNTIME("Shader cache desync");
    GLint framebuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &framebuffer);
    if (state.current_framebuffer != framebuffer)
        THROW_RUNTIME("Framebuffer cache desync");
    GLint vertex_array;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vertex_array);
    if (state.current_vertex_array != vertex_array)
        THROW_RUNTIME("Vertex array cache desync");
    GLint texture;
    for (int i = 1; i < GLState::maxTextureUnits; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture);
        if (state.bound_textures[i] != static_cast<GLuint>(texture)) {
            THROW_RUNTIME("Texture cache desync at slot " + std::to_string(i));
        }
    }
}

GLContext::~GLContext() {
    if (wglGetCurrentContext() == static_cast<HGLRC>(m_glrc)) {
        wglMakeCurrent(nullptr, nullptr);
    }

    if (static_cast<HGLRC>(m_glrc)) {
        wglDeleteContext(static_cast<HGLRC>(m_glrc));
        m_glrc = nullptr;
    }
}

void GLContext::make_current() const {
    if (current_context == this)
        return;
    wglMakeCurrent(static_cast<HDC>(m_hdc), static_cast<HGLRC>(m_glrc));
    current_context = const_cast<GLContext*>(this);
}

void GLContext::clear_current() {
    if (!current_context)
        return;
    wglMakeCurrent(nullptr, nullptr);
    current_context = nullptr;
}

void GLContext::swap_buffers() const {
    SwapBuffers(static_cast<HDC>(m_hdc));
}

[[nodiscard]] bool GLContext::supports_dsa() const { return supports_DSA; }

