
#include "BezierGroup.h"

#include <utility>

#include "rendercore/mesh/Mesh.h"
#include "rendercore/surface/Color.h"

BezierGroup::BezierGroup(const std::vector<Bezier> &curves, const std::vector<Color> &colors) {
    for (const auto &curve : curves) {
        add_curve(curve, colors[0]);
    }
}

void BezierGroup::add_curve(const Bezier &curve, const Color &color) {
    std::visit([&]<typename T>(const T& c) {
        using Type = std::decay_t<T>;
        if constexpr (std::is_same_v<Type, CubicBezierCurve>) {
            cubic_curves.push_back(c);
            cubic_colors.push_back(color);
        }
        else {
            quadratic_curves.push_back(c);
            quadratic_colors.push_back(color);
        }
    }, curve);
}

void BezierGroup::build_cubic_mesh() {
    if (is_cubic_dirty) {
        cubic_mesh = Mesh::create();
        std::vector<float> curve_data(cubic_curves.size() * 8);
        for (int i = 0; i < cubic_curves.size() ; ++i) {
            auto points = cubic_curves[i].get_control_points();
            for (int j = 0; j < points.size(); ++j) {
                curve_data[i * 8 + j * 2] = points[j].x;
                curve_data[i * 8 + j * 2 + 1] = points[j].y;
            }
        }

        const auto curve_buf = ArrayBuffer::create(curve_data);
        curve_buf->add_attribute(
        ArrayBufferAttribute(2, 0)
            .set_offset(0)
            .set_divisor(1)
        );

        curve_buf->add_attribute(
            ArrayBufferAttribute(2, 1)
                .set_offset(2)
                .set_divisor(1)
        );

        curve_buf->add_attribute(
            ArrayBufferAttribute(2, 2)
                .set_offset(4)
                .set_divisor(1)
        );
        curve_buf->add_attribute(
            ArrayBufferAttribute(2, 3)
                .set_offset(6)
                .set_divisor(1)
        );
        std::vector<float> widths(cubic_curves.size());
        for (int i = 0; i < cubic_colors.size() ; ++i) {
            widths[i] = cubic_curves[i].width;
        }
        const ArrayBufferAttribute width_attribute = ArrayBufferAttribute(1, 4).set_divisor(1);
        const auto width_buf = ArrayBuffer::create(widths);
        width_buf->add_attribute(width_attribute);

        std::vector<float> color_data(cubic_colors.size() * 4);
        for (int i = 0; i < cubic_colors.size() ; ++i) {
            color_data[4 * i] = cubic_colors[i].r;
            color_data[4 * i + 1] = cubic_colors[i].g;
            color_data[4 * i + 2] = cubic_colors[i].b;
            color_data[4 * i + 3] = cubic_colors[i].a;
        }
        const ArrayBufferAttribute color_attribute = ArrayBufferAttribute(4, 5).set_divisor(1);
        const auto color_buf = ArrayBuffer::create(color_data);
        color_buf->add_attribute(color_attribute);
        cubic_mesh->add_instance_buffer(curve_buf);
        cubic_mesh->add_instance_buffer(width_buf);
        cubic_mesh->add_instance_buffer(color_buf);
        is_cubic_dirty = false;
    }
}

void BezierGroup::build_quadratic_mesh() {
    if (is_quadratic_dirty) {
        quadratic_mesh = Mesh::create_line(0, 0, 1, 1);
        std::vector<float> curve_data(quadratic_curves.size() * 8);
        for (int i = 0; i < quadratic_curves.size() ; ++i) {
            auto points = quadratic_curves[i].get_control_points();
            for (int j = 0; j < points.size(); ++j) {
                curve_data[i * 8 + j * 2] = points[j].x;
                curve_data[i * 8 + j * 2 + 1] = points[j].y;
            }
        }

        const auto curve_buf = ArrayBuffer::create(curve_data);
        curve_buf->add_attribute(
        ArrayBufferAttribute(2, 0)
            .set_offset(0)
            .set_divisor(1)
        );

        curve_buf->add_attribute(
            ArrayBufferAttribute(2, 1)
                .set_offset(2)
                .set_divisor(1)
        );

        curve_buf->add_attribute(
            ArrayBufferAttribute(2, 2)
                .set_offset(4)
                .set_divisor(1)
        );
        std::vector<float> color_data(quadratic_colors.size() * 4);
        for (int i = 0; i < quadratic_colors.size() ; ++i) {
            color_data[i * 4] = quadratic_colors[i].r;
            color_data[i * 4 + 1] = quadratic_colors[i].g;
            color_data[i * 4 + 2] = quadratic_colors[i].b;
            color_data[i * 4 + 3] = quadratic_colors[i].a;
        }
        const ArrayBufferAttribute color_attribute = ArrayBufferAttribute(4, 3).set_divisor(1);
        const auto color_buf = ArrayBuffer::create(color_data);
        color_buf->add_attribute(color_attribute);
        quadratic_mesh->add_instance_buffer(curve_buf);
        quadratic_mesh->add_instance_buffer(color_buf);
        is_quadratic_dirty = false;
    }
}

std::shared_ptr<Mesh> BezierGroup::get_cubic_mesh() {
    build_cubic_mesh();
    return cubic_mesh;
}

std::shared_ptr<Mesh> BezierGroup::get_quadratic_mesh() {
    build_quadratic_mesh();
    return quadratic_mesh;
}

