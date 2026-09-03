
#include "ArrayBuffer.h"

#include <cstring>

#include "rendercore/opengl/GLTrack.h"


std::shared_ptr<ArrayBuffer> ArrayBuffer::create(const std::vector<float> &data) {
    return std::shared_ptr<ArrayBuffer>(new ArrayBuffer(data));
}


ArrayBuffer::~ArrayBuffer() {
    glDeleteBuffers(1, &m_id);
}

void ArrayBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void ArrayBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


std::shared_ptr<ArrayBuffer> ArrayBuffer::set_data(const std::vector<float>& new_data, const DrawMode usage) {
    this->data = new_data;
    if (is_built) {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, new_data.size() * sizeof(float), new_data.data(), to_gl(usage));
        m_group_count = new_data.size() / attributes[0].size;
    }
    return shared_from_this();
}
void ArrayBuffer::update_data(const std::vector<float>& new_data, const int start) {
    if (!is_built)
        throw std::runtime_error("To update buffer data first call build() and use set_data() to first of all allocate memory!");
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    size_t byte_size = std::min(this->data.size() - start, new_data.size())*sizeof(float);
    int start_bytes = start * sizeof(float);
    glBufferSubData(GL_ARRAY_BUFFER, start_bytes, byte_size, new_data.data());
    std::memcpy(this->data.data() + start, new_data.data(), byte_size);
    m_group_count = this->data.size() / attributes[0].size;
}
std::shared_ptr<ArrayBuffer> ArrayBuffer::add_attribute(ArrayBufferAttribute attribute) {
    attributes.push_back(attribute);
    return shared_from_this();
}
std::shared_ptr<ArrayBuffer> ArrayBuffer::set_usage(unsigned int usage) {
    this->usage = usage;
    return shared_from_this();
}

std::shared_ptr<ArrayBuffer> ArrayBuffer::build(const GLuint vao) {
    bind_vao(vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), usage);

    m_stride = 0;
    for (const auto& attribute : attributes) {
        int attrib_stride = attribute.size + attribute.offset;
        if (attrib_stride > m_stride) {
            m_stride = attrib_stride;
        }
    }

    int stride_bytes = m_stride * sizeof(float);
    m_group_count = data.size() / m_stride;

    for (const auto& attribute : attributes) {
        glVertexAttribPointer(
            attribute.layout,
            attribute.size,
            GL_FLOAT,
            GL_FALSE,
            stride_bytes,
            reinterpret_cast<void *>(attribute.offset * sizeof(float))
        );

        glEnableVertexAttribArray(attribute.layout);
        glVertexAttribDivisor(attribute.layout, attribute.divisor);
    }

    is_built = true;
    return shared_from_this();
}




ArrayBuffer::ArrayBuffer(std::vector<float> data) {
    this->data = std::move(data);
    glGenBuffers(1, &m_id);
}

