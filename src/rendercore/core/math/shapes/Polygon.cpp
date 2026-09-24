
#include "Polygon.h"

#include <algorithm>
#include <optional>

#include "rendercore/core/Warnings.h"
#include "rendercore/mesh/Mesh.h"


static size_t find_rightmost_point(const std::vector<Vec2>& points) {
    float largest_x = points[0].x;
    size_t result = 0;
    for (size_t i = 1; i < points.size(); ++i)
        if (points[i].x > largest_x) {
            largest_x = points[i].x;
            result = i;
        }
    return result;
}

Vec2 OuterPolygonContour::get_rightmost() const {
    if (has_rightmost)
        return rightmost;
    rightmost_index = find_rightmost_point(vertices);
    rightmost = vertices[rightmost_index];
    has_rightmost = true;
    return rightmost;
}

Vec2 InnerPolygonContour::get_rightmost() const {
    if (has_rightmost)
        return rightmost;
    rightmost_index = find_rightmost_point(vertices);
    rightmost = vertices[rightmost_index];
    has_rightmost = true;
    return rightmost;
}

void Polygon::build_mesh() {
    mesh = Mesh::create();
    std::vector<Line> lines;
}

std::shared_ptr<Mesh> Polygon::get_mesh() {
    build_mesh();
    return mesh;
}



OuterPolygonContour Polygon::generate_bridged_contour() {
    OuterPolygonContour result = outer_contour;
    sort_polygon_contours();
    for (size_t i=0; i < inner_polygon_contours.size(); ++i) {
        const auto& contour = inner_polygon_contours[i];
        // search
        const auto start = contour.get_rightmost();
        auto ray = Line(start.x, start.y, outer_contour.get_rightmost().x, start.y);
        std::optional<Vec2> nearest_intersection = std::nullopt;
        size_t nearest_intersection_vertex_index = 0;
        for (size_t l_i = 0; l_i < result.vertices.size(); ++l_i) {
            const auto& p0 = result.vertices[l_i];
            const auto& p1 = result.vertices[(l_i + 1) % result.vertices.size()];
            Line line(p0.x, p0.y, p1.x, p1.y);
            if (const auto& [is_intersecting, intersection] = ray.intersection(line); is_intersecting && (!nearest_intersection || intersection.x < nearest_intersection.value().x)) {
                nearest_intersection = intersection;
                nearest_intersection_vertex_index = l_i;
            }
        }
        if (!nearest_intersection)
            THROW_RUNTIME("Your inner contour is invalid!")
        // insertion
        std::vector<Vec2> joined_lines(result.vertices.size() + contour.vertices.size() + 3);
        memcpy(joined_lines.data(), result.vertices.data(), nearest_intersection_vertex_index * sizeof(Vec2));
        const auto& p0 = result.vertices[nearest_intersection_vertex_index];
        const auto& p1 = result.vertices[(nearest_intersection_vertex_index + 1) % result.vertices.size()];
        joined_lines[nearest_intersection_vertex_index] = p0;
        joined_lines[nearest_intersection_vertex_index + 1] = nearest_intersection.value();

    }
    return result;
}

void Polygon::sort_polygon_contours() {
    std::ranges::stable_sort(inner_polygon_contours,
         [](const InnerPolygonContour& a, const InnerPolygonContour& b) {
             return a.get_rightmost().x > b.get_rightmost().x;
         }
    );
}
