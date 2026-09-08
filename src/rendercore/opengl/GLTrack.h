#ifndef GLTRACK_H
#define GLTRACK_H
#include "GLContext.h"
#include "../core/Warnings.h"
#include "glad/glad.h"
#include <iostream>
#include <stdexcept>


inline void bind_framebuffer(const GLuint fbo) {
    if (!GLContext::current_context) {
        THROW_RUNTIME("GLContext not initialized");
    }
    if (GLContext::current_context->state.current_framebuffer == fbo) {
#ifdef DEBUG_CACHE
        GLContext::current_context->validate_state();
#endif
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    GLContext::current_context->state.current_framebuffer = fbo;
}

inline void use_program(const GLint program) {
    if (!GLContext::current_context) {
        THROW_RUNTIME("GLContext not initialized");
    }
    if (GLContext::current_context->state.current_shader == program) {
#ifdef DEBUG_CACHE
        GLContext::current_context->validate_state();
#endif
        return;
    }
    glUseProgram(program);
    GLContext::current_context->state.current_shader = program;
}

inline void bind_vao(const GLuint vao) {
    if (!GLContext::current_context) {
        THROW_RUNTIME("GLContext not initialized");
    }
    if (GLContext::current_context->state.current_vertex_array == vao) {
#ifdef DEBUG_CACHE
        GLContext::current_context->validate_state();
#endif
        return;
    }
    glBindVertexArray(vao);
    GLContext::current_context->state.current_vertex_array = vao;
}

inline void bind_sampler(const GLuint unit, const GLuint sampler) {
    if (!GLContext::current_context) {
        THROW_RUNTIME("GLContext not initialized");
    }
    if (GLContext::current_context->state.bound_samplers[unit] == sampler) {
#ifdef DEBUG_CACHE
        GLContext::current_context->validate_state();
#endif
        return;
    }
    glBindSampler(unit, sampler);
    GLContext::current_context->state.bound_samplers[unit] = sampler;
}

inline void bind_texture(const GLuint unit, const GLuint tex) {
    if (!GLContext::current_context) {
        THROW_RUNTIME("GLContext not initialized");
    }
    if (GLContext::current_context->state.bound_textures[unit] == tex) {
#ifdef DEBUG_CACHE
        GLContext::current_context->validate_state();
#endif
        return;
    }
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tex);
    GLContext::current_context->state.bound_textures[unit] = tex;
}

inline void set_pixel_unpack_alignment(const int alignment) {
    if (!GLContext::current_context) {
        THROW_RUNTIME("GLContext not initialized");
    }
    if (GLContext::current_context->state.pixel_unpack_alignment == alignment) {
#ifdef DEBUG_CACHE
        GLContext::current_context->validate_state();
#endif
        return;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

    GLContext::current_context->state.pixel_unpack_alignment = alignment;
}

#endif //GLTRACK_H
