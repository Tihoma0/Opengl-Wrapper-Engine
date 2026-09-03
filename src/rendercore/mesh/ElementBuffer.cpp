
#include "ElementBuffer.h"

#include <memory>
#include <vector>

#include "glad/glad.h"
#include "rendercore/core/enums/textures/DrawMode.h"
#include "rendercore/opengl/GLTrack.h"


std::shared_ptr<ElementBuffer> ElementBuffer::create(const std::vector<int> &data, const DrawMode draw_mode) {
    return std::shared_ptr<ElementBuffer>(new ElementBuffer(data, draw_mode));
}

void ElementBuffer::build(const GLuint vao) {
    if (is_built) {
        throw std::runtime_error("ElementBuffer already built");
    }
    bind_vao(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(int), data.data(), to_gl(usage));
    is_built = true;
}


ElementBuffer::ElementBuffer(std::vector<int> data, DrawMode draw_mode) {
    glGenBuffers(1, &m_id);
    this->usage = draw_mode;
    this->data = std::move(data);
}