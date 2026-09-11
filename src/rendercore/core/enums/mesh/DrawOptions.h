#ifndef DRAWOPTIONS_H
#define DRAWOPTIONS_H

#include "glad/glad.h"

static constexpr int NO_VALUE = -1;

struct DrawOptions {
    int m_instance_count = NO_VALUE;
    int m_vertex_count = NO_VALUE;
    int m_index_count = NO_VALUE;
    int m_vertex_offset = 0;
    int m_instance_offset = 0;
    GLenum m_draw_mode = GL_TRIANGLES;

    DrawOptions() = default;

    explicit DrawOptions(const GLenum draw_mode, const int vertex_offset = 0, const int vertex_count = NO_VALUE) : m_vertex_count(vertex_count), m_vertex_offset(vertex_offset), m_draw_mode(draw_mode) {}

    DrawOptions& vertex_count(const int vertex_count) { this->m_vertex_count = vertex_count; return *this; }

    DrawOptions& vertex_offset(const int vertex_offset) { this->m_vertex_offset = vertex_offset; return *this; }

    DrawOptions& instance_count(const int instance_count) { this->m_instance_count = instance_count; return *this; }

    DrawOptions& instance_offset(const int instance_offset) { this->m_instance_offset = instance_offset; return *this; }

    DrawOptions& index_count(const int index_count) { this->m_index_count = index_count; return *this; }

    DrawOptions& draw_mode(const GLenum draw_mode) { this->m_draw_mode = draw_mode; return *this; }
};
#endif //DRAWOPTIONS_H
