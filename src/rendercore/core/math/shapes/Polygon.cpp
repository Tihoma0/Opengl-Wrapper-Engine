
#include "Polygon.h"

#include "rendercore/mesh/Mesh.h"

Polygon::Polygon(const std::vector<PolygonContour> &contours) {
    this->contours = contours;
}

Line get_bridge(const PolygonContour& contour, const PolygonContour& other) {
    const auto result = Line(0, 0, 0, 0);
    for (auto line: contour.lines) {
        for (auto other_line: other.lines) {
            Vec2 dist1 = other

        .}
    }
    return result;
}

void Polygon::build_mesh() {
    mesh = Mesh::create();
    std::vector<Line> lines;
    for (auto& contour : contours) {

    }
}

std::shared_ptr<Mesh> Polygon::get_mesh() {
    build_mesh();
    return mesh;
}
