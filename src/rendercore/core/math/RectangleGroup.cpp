//
// Created by marku on 02.09.2026.
//

#include "RectangleGroup.h"


RectangleGroup::RectangleGroup(const std::vector<Rectangle> &rectangles) {
    this->rects = rectangles;
}

void DebugBoundVAO() {
    GLint currentVAO = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);

    if (currentVAO == 0) {
        std::cout << "[VAO Debug] WARNING: No VAO is currently bound!\n";
        return;
    }

    std::cout << "========================================\n";
    std::cout << "[VAO Debug] Checking VAO ID: " << currentVAO << "\n";
    std::cout << "========================================\n";

    GLint maxAttribs = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);

    for (int i = 0; i < maxAttribs; ++i) {
        GLint enabled = 0;
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);

        // Skip attribute slots that aren't turned on
        if (!enabled) continue;

        GLint size = 0, stride = 0, type = 0, normalized = 0, bufferBinding = 0;
        void* pointerOffset = nullptr;

        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &normalized);
        glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &bufferBinding);
        glGetVertexAttribPointerv(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, &pointerOffset);

        std::cout << "Attribute Location [" << i << "]:\n"
                  << "  -> Enabled:    YES\n"
                  << "  -> Size:       " << size << " components\n"
                  << "  -> Type:       0x" << std::hex << type << std::dec << " (GL_FLOAT is 0x1406)\n"
                  << "  -> Normalized: " << (normalized ? "TRUE" : "FALSE") << "\n"
                  << "  -> Stride:     " << stride << " bytes\n"
                  << "  -> Offset:     " << pointerOffset << "\n"
                  << "  -> Bound VBO:  ID " << bufferBinding << "\n\n";
    }

    GLint eboBinding = 0;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eboBinding);
    std::cout << "  -> Bound EBO (Index Buffer): ID " << eboBinding << "\n";
    std::cout << "========================================\n";
}

void RectangleGroup::build_mesh() {
    if (!is_dirty)
        return;
    this->mesh = Mesh::create_quad(0, 0, 1, 1);
    std::vector<float> data(rects.size() * 4);
    for (int i = 0; i < rects.size(); ++i) {
        data[i * 4] = rects[i].x;
        data[i * 4 + 1] = rects[i].y;
        data[i * 4 + 2] = rects[i].w;
        data[i * 4 + 3] = rects[i].h;
    }
    const ArrayBufferAttribute attribute = ArrayBufferAttribute(4, 1).set_offset(0).set_divisor(1);
    const std::shared_ptr<ArrayBuffer> buf = ArrayBuffer::create(data);
    buf->add_attribute(attribute);
    mesh->add_instance_buffer(buf);
    is_dirty = false;
}

void RectangleGroup::add_rectangle(const Rectangle &rectangle) {
    rects.push_back(rectangle);
}