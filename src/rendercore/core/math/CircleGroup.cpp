
#include "CircleGroup.h"
#include "Circle.h"

CircleGroup::CircleGroup(const std::vector<Circle> &circles) {
    this->circles = circles;
}

void CircleGroup::add_circle(const Circle &circle) {
    this->circles.push_back(circle);
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
    mesh->add_instance_buffer(buf);
    is_dirty = false;
}

