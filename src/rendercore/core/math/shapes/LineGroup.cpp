
#include "LineGroup.h"

#include <iostream>

#include "Line.h"
#include "rendercore/surface/Color.h"

LineGroup::LineGroup(const std::vector<Line> &lines, const std::vector<Color> &colors) {
    this->lines = lines;
    this->colors = colors;
}

LineGroup::LineGroup() {
    this->lines = std::vector<Line>();
    this->colors = std::vector<Color>();
    this->is_quad_dirty = true;
    is_line_dirty = true;
}

void LineGroup::add(const Line &line, const Color &color) {
    this->lines.push_back(line);
    this->colors.push_back(color);
    this->is_quad_dirty = true;
    is_line_dirty = true;
}

void LineGroup::build_line_mesh() {
    if (!is_line_dirty)
        return;
    line_mesh = Mesh::create_line(0, 0, 1, 1);
    std::vector<float> data(lines.size() * 4);
    for (int i = 0; i < lines.size(); ++i) {
        data[i*4] = lines[i].x1;
        data[i*4 + 1] = lines[i].y1;
        const auto vector = lines[i].along();
        data[i*4 + 2] = vector.x;
        data[i*4 + 3] = vector.y;
    }
    const std::shared_ptr<ArrayBuffer> buf = ArrayBuffer::create(data);
    const ArrayBufferAttribute attribute = ArrayBufferAttribute(4, 1).set_offset(0).set_divisor(1);
    buf->add_attribute(attribute);
    std::vector<float> color_data(colors.size() * 4);
    for (int i = 0; i < colors.size(); ++i) {
        color_data[i*4] = colors[i].r;
        color_data[i*4 + 1] = colors[i].g;
        color_data[i*4 + 2] = colors[i].b;
        color_data[i*4 + 3] = colors[i].a;
    }
    const std::shared_ptr<ArrayBuffer> color_buf = ArrayBuffer::create(color_data);
    const ArrayBufferAttribute color_attribute = ArrayBufferAttribute(4, 2).set_offset(0).set_divisor(1);
    color_buf->add_attribute(color_attribute);
    line_mesh->add_instance_buffer(buf);
    line_mesh->add_instance_buffer(color_buf);
    is_line_dirty = false;
}

void LineGroup::build_quad_mesh() {
    if (!is_quad_dirty)
        return;
    quad_mesh = Mesh::create_quad(-0.5, -0.5, 1, 1);
    std::vector<float> data(lines.size() * 4);
    for (int i = 0; i < lines.size(); ++i) {
        data[i*4] = lines[i].x1;
        data[i*4 + 1] = lines[i].y1;
        const auto normal = lines[i].along().normalize();
        data[i*4 + 2] = normal.x;
        data[i*4 + 3] = normal.y;
    }
    const std::shared_ptr<ArrayBuffer> buf = ArrayBuffer::create(data);
    const ArrayBufferAttribute attribute = ArrayBufferAttribute(4, 1).set_offset(0).set_divisor(1);
    buf->add_attribute(attribute);
    std::vector<float> size_data(lines.size() * 2);
    for (int i = 0; i < lines.size(); ++i) {
        size_data[i*2] = lines[i].along().length();
        size_data[i*2 + 1] = lines[i].width;
    }
    const std::shared_ptr<ArrayBuffer> size_buf = ArrayBuffer::create(size_data);
    const ArrayBufferAttribute size_attribute = ArrayBufferAttribute(2, 2).set_offset(0).set_divisor(1);
    size_buf->add_attribute(size_attribute);
    std::vector<float> color_data(colors.size() * 4);
    for (int i = 0; i < colors.size(); ++i) {
        color_data[i*4] = colors[i].r;
        color_data[i*4 + 1] = colors[i].g;
        color_data[i*4 + 2] = colors[i].b;
        color_data[i*4 + 3] = colors[i].a;
    }
    const std::shared_ptr<ArrayBuffer> color_buf = ArrayBuffer::create(color_data);
    const ArrayBufferAttribute color_attribute = ArrayBufferAttribute(4, 3).set_offset(0).set_divisor(1);
    color_buf->add_attribute(color_attribute);
    quad_mesh->add_instance_buffer(buf);
    quad_mesh->add_instance_buffer(color_buf);
    quad_mesh->add_instance_buffer(size_buf);
    is_quad_dirty = false;
}

void LineGroup::clear() {
    colors.clear();
    lines.clear();
    is_quad_dirty = true;
}

Line& LineGroup::get_line(const int index) {
    return lines[index];
}

Color& LineGroup::get_color(const int index) {
    return colors[index];
}

void LineGroup::set(const int i, const Line &Line, const Color &color) {
    lines[i] = Line;
    colors[i] = color;
    is_quad_dirty = true;
    is_line_dirty = true;
}

void LineGroup::set(const int i, const Color &color) {
    colors[i] = color;
    is_quad_dirty = true;
    is_line_dirty = true;
}

void LineGroup::set(const int i, const Line &line) {
    lines[i] = line;
    is_quad_dirty = true;
    is_line_dirty = true;
}

void LineGroup::remove(const int index) {
    lines.erase(lines.begin() + index);
    colors.erase(colors.begin() + index);
    is_quad_dirty = true;
    is_line_dirty = true;
}

