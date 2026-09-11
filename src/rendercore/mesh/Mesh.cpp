#include <iostream>
#include <memory>
#include <vector>

#include <rendercore/mesh/ArrayBuffer.h>
#include <rendercore/mesh/ElementBuffer.h>
#include <glad/glad.h>

#include "../opengl/GLTrack.h"
#include "../core/Warnings.h"
#include "rendercore/mesh/Mesh.h"


std::shared_ptr<Mesh> Mesh::create() {
    return std::shared_ptr<Mesh>(new Mesh());
}

std::shared_ptr<Mesh> Mesh::create_quad(const float x, const float y, const float width, const float height) {
    const std::vector quad_vertices = {
        x, y,
        x, y + height,
        x + width, y + height,
        x, y,
        x + width, y + height,
        x + width, y
    };
    const auto quad_buffer = ArrayBuffer::create(quad_vertices);
    const ArrayBufferAttribute attribute = ArrayBufferAttribute(2, 0).set_offset(0);
    quad_buffer->add_attribute(attribute);
    auto mesh_ptr = create();
    mesh_ptr -> add_vertex_buffer(quad_buffer);
    return mesh_ptr;
}

std::shared_ptr<Mesh> Mesh::create_line(const float x1, const float y1, const float x2, const float y2) {
    const std::vector line_vertices = {
        x1, y1,
        x2, y2
    };
    const auto line_buffer = ArrayBuffer::create(line_vertices);
    const ArrayBufferAttribute attribute = ArrayBufferAttribute(2, 0).set_offset(0);
    line_buffer->add_attribute(attribute);
    auto mesh_ptr = create();
    mesh_ptr -> add_vertex_buffer(line_buffer);
    return mesh_ptr;
}


Mesh::~Mesh(){
    glDeleteVertexArrays(1, &vao_id);
}

std::shared_ptr<Mesh> Mesh::add_vertex_buffer(const std::shared_ptr<ArrayBuffer>& buffer) {
    buffer->build(vao_id);
    vertex_vbos.emplace_back(buffer);
    if (warnings.mesh_vertex_mismatch && m_default_vertex_count != 0 && m_default_vertex_count != buffer->group_count())
        std::cerr << "Warning: your Array Buffers do not have the same vertex count!";
    m_default_vertex_count = buffer->group_count();
    return shared_from_this();
}

std::shared_ptr<Mesh> Mesh::add_instance_buffer(const std::shared_ptr<ArrayBuffer>& buffer) {
    buffer->build(vao_id);
    instance_vbos.emplace_back(buffer);
    if (m_default_instance_count == 0) {
        m_default_instance_count = buffer->group_count();
    } else if (m_default_instance_count != buffer->group_count())
            std::cerr << "Warning: your Array Buffers do not have the same instance count!";
    return shared_from_this();
}

std::shared_ptr<Mesh> Mesh::add_element_buffer(const std::shared_ptr<ElementBuffer> &buffer) {
    buffer->build(vao_id);
    ebo = buffer;
    m_default_index_count = buffer->size();
    has_ebo = true;
    return shared_from_this();
}

void Mesh::bind() const {
    bind_vao(vao_id);
}

void Mesh::unbind() const {
    bind_vao(0);
}


