
#include "LineGroup.h"
#include "Line.h"
#include "rendercore/surface/Color.h"

LineGroup::LineGroup(const std::vector<Line> &lines, const std::vector<Color> &colors) {
    this->lines = lines;
    this->colors = colors;
}

void LineGroup::add(const Line &line, const Color &color) {
    this->lines.push_back(line);
    this->colors.push_back(color);
    this->is_dirty = true;
}

void LineGroup::build_mesh() {
    mesh = Mesh::create_quad(0, 0, 1, 1);
    std::vector<float> data(lines.size() * 4);
    for (int i = 0; i < lines.size(); ++i) {
        data[i*3] = lines[i].x1;
        data[i*3+1] = lines[i].y1;
        data[i*3+2] = lines[i].x2;
        data[i*3+3] = lines[i].y2;
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
    mesh->add_instance_buffer(buf);
    mesh->add_instance_buffer(color_buf);
    is_dirty = false;
}

void LineGroup::clear() {
    colors.clear();
    lines.clear();
    is_dirty = true;
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
    is_dirty = true;
}

void LineGroup::set(const int i, const Color &color) {
    colors[i] = color;
    is_dirty = true;
}

void LineGroup::set(const int i, const Line &line) {
    lines[i] = line;
    is_dirty = true;
}

void LineGroup::remove(const int index) {
    lines.erase(lines.begin() + index);
    colors.erase(colors.begin() + index);
    is_dirty = true;
}

