#include "rendercore/render/Renderer.h"

#include <memory>

#include "../material/Material.h"
#include "RenderTarget.h"
#include "Framebuffer.h"
#include "rendercore/core/enums/mesh/DrawOptions.h"
#include "rendercore/mesh/Mesh.h"

thread_local Color current_clear_color;
thread_local std::shared_ptr<RenderTarget> render_target;

void Renderer::clear(const Color color) {
    if (current_clear_color != color) {
        glClearColor(color.r, color.g, color.b, color.a);
        current_clear_color = color;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::set_target(const std::shared_ptr<RenderTarget> &target) {
    render_target = target;
}

void Renderer::draw(const std::shared_ptr<Mesh>& mesh, Material& material, const DrawOptions& options) {
    if (render_target)
        render_target->bind();
    material.upload();
    mesh->bind();
#ifdef DEBUG_CACHE
    GLContext::current_context->validate_state();
#endif
    int vertex_count = options.m_vertex_count;
    int instance_count = options.m_instance_count;
    if (options.m_vertex_count == NO_VALUE) {
        vertex_count = mesh->default_vertex_count();
    }
    if (options.m_instance_count == NO_VALUE) {
        instance_count = mesh->default_instance_count();
    }
    int index_count = mesh->default_index_count();
    if (options.m_index_count != NO_VALUE) {
        index_count = options.m_index_count;
    }
    if (instance_count == 0) {
        if (mesh->has_element_buffer())
            glDrawElements(options.m_draw_mode, vertex_count, GL_UNSIGNED_INT, reinterpret_cast<void *>(options.m_vertex_offset * sizeof(GLuint)));
        else
            glDrawArrays(options.m_draw_mode, options.m_vertex_offset, vertex_count);
    }
    else {
        if (mesh->has_element_buffer() && options.m_instance_offset != NO_VALUE)
            glDrawElementsInstancedBaseInstance(options.m_draw_mode, index_count, GL_UNSIGNED_INT, reinterpret_cast<void *>(options.m_vertex_offset * sizeof(GLuint)), instance_count, options.m_instance_offset);
        else if (mesh->has_element_buffer())
            glDrawElementsInstanced(options.m_draw_mode, index_count, GL_UNSIGNED_INT, reinterpret_cast<void *>(options.m_vertex_offset * sizeof(GLuint)), instance_count);
        else if (options.m_instance_offset != NO_VALUE)
            glDrawArraysInstancedBaseInstance(options.m_draw_mode, options.m_vertex_offset, vertex_count, instance_count, options.m_instance_offset);
        else
            glDrawArraysInstanced(options.m_draw_mode, options.m_vertex_offset, vertex_count, instance_count);
    }
}
