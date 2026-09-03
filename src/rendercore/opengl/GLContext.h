#ifndef CONTEXT_H
#define CONTEXT_H
#include <memory>
#include <vector>

#include <glad/glad.h>

#include "../material/ShaderCache.h"

#include "Blendfunc.h"



class Sampler;
class GLContext;
class Texture2D;
class Mesh;
class Shader;



struct GLLimits {
    GLint maxTextureDimensions = 0;
    GLint max3DTextureSize = 0;
    GLint maxTextureUnits = 0;
    GLint maxCombinedTextureUnits = 0;
    GLint maxColorAttachments = 0;
    GLint maxDrawBuffers = 0;
    GLint maxVertexUniforms = 0;
    GLint maxFragmentUniforms = 0;
};


struct GLState {
    inline static int maxTextureUnits = 0;
    GLint current_shader = 0;
    GLuint current_framebuffer = 0;
    GLuint current_vertex_array = 0;
    std::vector<GLuint> bound_textures = std::vector<GLuint>(maxTextureUnits);
    std::vector<GLuint> bound_samplers = std::vector<GLuint>(maxTextureUnits);
    std::shared_ptr<Sampler> default_sampler = nullptr;

};

class GLContext {

public:
    inline static thread_local GLContext* current_context = nullptr;

    GLLimits limits = {};
    GLState state;
    ShaderCache shader_cache;
    int mayor_version = 0;
    int minor_version = 0;

    GLContext(void* hwnd, int glMayorVersion, int glMinorVersion, const GLContext* shared_context = nullptr);


    [[nodiscard]] bool at_least_version(int major, int minor) const;

    [[nodiscard]] GLLimits query_limits();

    void invalidate_state();

    void validate_state() const;

    ~GLContext();

    void make_current() const;

    void setBlend(const BlendState &new_blend) {
        if (blend_state != new_blend) {
            new_blend.apply();
            blend_state = new_blend;
        }
    }

    static void clear_current();

    void swap_buffers() const;

    [[nodiscard]] bool supports_dsa() const;


private:
    void* m_glrc = nullptr;
    void* m_hdc = nullptr;
    bool supports_DSA = false;
    BlendState blend_state;
};


#endif //CONTEXT_H
