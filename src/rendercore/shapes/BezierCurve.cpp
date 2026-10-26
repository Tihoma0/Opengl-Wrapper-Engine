
#include "BezierCurve.h"

#include <iostream>
#include <vector>

#include "rendercore/core/math/vec/Vec2.h"
#include "rendercore/mesh/Mesh.h"


[[nodiscard]] Vec2 CubicBezierCurve::evaluate(const float t) const {
    const float u = 1.0f - t;
    return
        p1 * (u * u * u) +
        p2 * (3.0f * u * u * t) +
        p3 * (3.0f * u * t * t) +
        p4 * (t * t * t);
}

[[nodiscard]] float CubicBezierCurve::get_width() const { return width; }

[[nodiscard]] std::vector<Vec2> CubicBezierCurve::get_control_points() const { return { p1, p2, p3, p4 }; }

std::shared_ptr<Mesh> CubicBezierCurve::get_mesh() {
    build_mesh();
    return mesh;
}

void CubicBezierCurve::build_mesh() {
    if (!mesh) {
        mesh = Mesh::create();
        const auto buf = ArrayBuffer::create({
            p1.x, p1.y,
            p2.x, p2.y,
            p3.x, p3.y,
            p4.x, p4.y
        });
        const auto attribute = ArrayBufferAttribute(2, 0);
        buf->add_attribute(attribute);
        mesh->add_vertex_buffer(buf);
    }
}


[[nodiscard]] Vec2 QuadraticBezierCurve::evaluate(const float t) const {
    const float u = 1.0f - t;
    return
        p1 * (u * u) +
        p2 * (2.0f * u * t) +
        p3 * (t * t);
}

[[nodiscard]] float QuadraticBezierCurve::get_width() const { return width; }

[[nodiscard]] std::vector<Vec2> QuadraticBezierCurve::get_control_points() const { return { p1, p2, p3 }; }

[[nodiscard]] std::shared_ptr<Mesh> QuadraticBezierCurve::get_mesh() {
    build_mesh();
    return mesh;
}

void QuadraticBezierCurve::build_mesh() {
    if (!mesh) {
        mesh = Mesh::create();
        const auto buf = ArrayBuffer::create({
            p1.x, p1.y,
            p2.x, p2.y,
            p3.x, p3.y}
        );
        const auto attribute = ArrayBufferAttribute(2, 0);
        buf->add_attribute(attribute);
        mesh->add_vertex_buffer(buf);
    }
}




