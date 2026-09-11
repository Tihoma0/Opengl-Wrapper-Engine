
#include "CircleGroup.h"
#include "Circle.h"
#include "rendercore/surface/Color.h"

CircleGroup::CircleGroup(const std::vector<Circle> &circles, const std::vector<Color> &colors) {
    this->circles = circles;
    this->colors = colors;
}

void CircleGroup::add(const Circle &circle, const Color &color) {
    this->circles.push_back(circle);
    this->colors.push_back(color);
    this->is_dirty = true;
}

void CircleGroup::build_mesh() {
    mesh = Mesh::create_quad(0, 0, 1, 1);
    std::vector<float> data(circles.size() * 3);
    for (int i = 0; i < circles.size(); ++i) {
        data[i*3] = circles[i].x;
        data[i*3+1] = circles[i].y;
        data[i*3+2] = circles[i].radius;
    }
    const std::shared_ptr<ArrayBuffer> buf = ArrayBuffer::create(data);
    const ArrayBufferAttribute attribute = ArrayBufferAttribute(3, 1).set_offset(0).set_divisor(1);
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

void CircleGroup::clear() {
    colors.clear();
    circles.clear();
    is_dirty = true;
}

Circle& CircleGroup::get_circle(const int index) {
    return circles[index];
}

Color& CircleGroup::get_color(const int index) {
    return colors[index];
}

void CircleGroup::set(const int i, const Circle &Circle, const Color &color) {
    circles[i] = Circle;
    colors[i] = color;
    is_dirty = true;
}

void CircleGroup::set(const int i, const Color &color) {
    colors[i] = color;
    is_dirty = true;
}

void CircleGroup::set(const int i, const Circle &circle) {
    circles[i] = circle;
    is_dirty = true;
}

void CircleGroup::remove(const int index) {
    circles.erase(circles.begin() + index);
    colors.erase(colors.begin() + index);
    is_dirty = true;
}

