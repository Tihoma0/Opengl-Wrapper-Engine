
#include "StaticPolygon.h"

#include <algorithm>
#include <format>
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

ContourOrientation OuterPolygonContour::get_orientation() const {
    if (has_orientation)
        return orientation;
    float total = 0;
    for (size_t i = 0; i < vertices.size(); ++i) {
        const auto p0 = vertices[i];
        const auto p1 = vertices[(i + 1) % vertices.size()];
        total += (p1.x - p0.x) * (p1.y + p0.y);
    }
    orientation = total > 0 ? ContourOrientation::CounterClockwise : ContourOrientation::Clockwise;
    has_orientation = true;
    return orientation;
}

Vec2 InnerPolygonContour::get_rightmost() const {
    if (has_rightmost)
        return rightmost;
    rightmost_index = find_rightmost_point(vertices);
    rightmost = vertices[rightmost_index];
    has_rightmost = true;
    return rightmost;
}

ContourOrientation InnerPolygonContour::get_orientation() const {
    if (has_orientation)
        return orientation;
    float total = 0;
    for (size_t i = 0; i < vertices.size(); ++i) {
        const auto p0 = vertices[i];
        const auto p1 = vertices[(i + 1) % vertices.size()];
        total += (p1.x - p0.x) * (p1.y + p0.y);
    }
    orientation = total > 0 ? ContourOrientation::CounterClockwise : ContourOrientation::Clockwise;
    has_orientation = true;
    return orientation;
}

bool is_convex(const Vec2& prev, const Vec2& curr, const Vec2& next, const ContourOrientation winding) {
    const auto cross = (curr - prev).cross(next - curr);
    return winding == ContourOrientation::CounterClockwise ? cross < 0 : cross > 0;
}

bool is_inside_triangle(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& p, const ContourOrientation winding) {
    const auto cross0 = (b - a).cross(p - a);
    const auto cross1 = (c - b).cross(p - b);
    const auto cross2 = (a - c).cross(p - c);
    if (winding == ContourOrientation::Clockwise)
        return cross0 > 0 && cross1 > 0 && cross2 > 0;
    return cross0 < 0 && cross1 < 0 && cross2 < 0;
}


StaticPolygon::StaticPolygon(OuterPolygonContour outer_contour,
    const std::vector<InnerPolygonContour>& inner_polygon_contours, const float clean_up_precision): outer_contour(std::move(outer_contour)), inner_polygon_contours(inner_polygon_contours)
{
    clean_up_contours(clean_up_precision);
    // check_degeneracies(clean_up_precision);
    sort_polygon_contours();
    calculate_orientation();
    generate_bridged_contour();
    build_mesh();
}

void StaticPolygon::build_mesh() {
    constexpr float min_triangle_area = 1e-3f;
    constexpr float min_triangle_area2 = min_triangle_area * min_triangle_area;
    mesh = Mesh::create();
    if (is_degenerate)
    {
        std::cerr << "your mesh is degenerate" << std::endl;
        return;
    }
    std::vector<Vec2> remaining_vertices = bridged_contour.vertices;
    std::vector<float> data;
    calculate_orientation();
    bool should_break = false;
    while (remaining_vertices.size() > 2) {
        bool found_ear = false;
        Vec2 curr;
        Vec2 next;
        Vec2 prev;
        for (int i = 0; i < remaining_vertices.size(); ++i) {
            const size_t prev_i = (i - 1 + remaining_vertices.size()) % remaining_vertices.size();
            const size_t next_i = (i + 1) % remaining_vertices.size();
            prev = remaining_vertices[prev_i];
            curr = remaining_vertices[i];
            next = remaining_vertices[next_i];
            const float area2 = (curr - prev).cross(next - prev);

            if (std::abs(area2) <= min_triangle_area2)
            {
                remaining_vertices.erase(remaining_vertices.begin() + i);
                if (remaining_vertices.size() <= 2)
                {
                    should_break = true;
                    break;
                }
            }
            if (!is_convex(prev, curr, next, orientation)) {
                continue;
            }
            found_ear = true;
            for (size_t j = 0; j < remaining_vertices.size(); ++j) {
                if (j == prev_i || j == i || j == next_i)
                    continue;
                if (is_inside_triangle(prev, curr, next, remaining_vertices[j], orientation)) {
                    found_ear = false;
                    break;
                }
            }
            if (!found_ear)
                continue;
            data.push_back(prev.x);
            data.push_back(prev.y);
            data.push_back(curr.x);
            data.push_back(curr.y);
            data.push_back(next.x);
            data.push_back(next.y);
            remaining_vertices.erase(remaining_vertices.begin() + i);
            break;
        }
        if (should_break)
            break;
        if (!found_ear)
            THROW_RUNTIME("Failed to triangulate polygon");
    }
    const auto buf = ArrayBuffer::create(data);
    buf->add_attribute(ArrayBufferAttribute(2, 0));
    mesh->add_vertex_buffer(buf);
}


std::shared_ptr<Mesh> StaticPolygon::get_mesh() {
    return mesh;
}


void StaticPolygon::clean_up_contours(float precision)
{
    const float precision2 = precision * precision;
    std::vector<Vec2> new_vertices;
    const size_t num_vertices = outer_contour.vertices.size();
    new_vertices.reserve(num_vertices);
    for (size_t i = 0; i < num_vertices; ++i)
    {
        const auto before = outer_contour.vertices[(i - 1 + num_vertices) % num_vertices];
        const auto curr = outer_contour.vertices[i];
        const auto next = outer_contour.vertices[(i + 1) % num_vertices];
        const auto curr_to_next = next - curr;
        const auto before_to_next = before - next;
        const bool is_too_close = curr_to_next.length2() <= precision;
        const float cross = curr_to_next.cross(before_to_next);
        const bool is_colinear = cross * cross < precision2 * before_to_next.length2();
        const bool continues_forward =
            curr_to_next.dot(curr - before) >= 0.0f;
        if (!is_too_close && !(is_colinear && continues_forward))
            new_vertices.push_back(curr);
    }
    new_vertices.swap(outer_contour.vertices);
    for (size_t j = 0; j < inner_polygon_contours.size();)
    {
        auto& inner_polygon_contour = inner_polygon_contours[j];
        const size_t num_vertices = inner_polygon_contour.vertices.size();
        if (num_vertices <= 2)
        {
            inner_polygon_contours.erase(inner_polygon_contours.begin() + j);
        }
        else {
            std::vector<Vec2> new_inner_polygon_contour_vertices;
            new_inner_polygon_contour_vertices.reserve(num_vertices);
            for (size_t i = 0; i < num_vertices; ++i)
            {
                const auto before = inner_polygon_contour.vertices[(i - 1 + num_vertices) % num_vertices];
                const auto curr = inner_polygon_contour.vertices[i];
                const auto next = inner_polygon_contour.vertices[(i + 1) % num_vertices];
                const auto curr_to_next = next - curr;
                const auto before_to_next = before - next;
                const bool is_too_close = curr_to_next.length2() <= precision;
                const float cross = curr_to_next.cross(before_to_next);
                const bool is_colinear = cross * cross < precision2 * before_to_next.length2();
                const bool continues_forward =
                    curr_to_next.dot(curr - before) >= 0.0f;
                if (!is_too_close && !(is_colinear && continues_forward))
                    new_inner_polygon_contour_vertices.push_back(curr);
            }
            new_inner_polygon_contour_vertices.swap(inner_polygon_contour.vertices);
            j++;
        }
    }
}

void StaticPolygon::check_degeneracies(float precision)
{
    const size_t outer_contour_vertex_count = outer_contour.vertices.size();
    if (outer_contour_vertex_count <= 2)
    {
        is_degenerate = true;
        return;
    }
    float total = 0;
    for (size_t i = 0; i < outer_contour_vertex_count; ++i) {
        const auto p0 = outer_contour.vertices[i];
        const auto p1 = outer_contour.vertices[(i + 1) % outer_contour_vertex_count];
        total += (p1.x - p0.x) * (p1.y + p0.y);
    }
    if (total < precision)
    {
        is_degenerate = true;
        return;
    }
    for (size_t i = 0; i < inner_polygon_contours.size(); ++i)
    {
        size_t vertex_count = inner_polygon_contours[i].vertices.size();
        if (vertex_count <= 2)
            inner_polygon_contours.erase(inner_polygon_contours.begin() + i);
        else
        {
            total = 0;
            for (size_t i = 0; i < outer_contour_vertex_count; ++i) {
                const auto p0 = outer_contour.vertices[i];
                const auto p1 = outer_contour.vertices[(i + 1) % outer_contour_vertex_count];
                total += (p1.x - p0.x) * (p1.y + p0.y);
            }
            if (total < precision)
                inner_polygon_contours.erase(inner_polygon_contours.begin() + i);
        }
    }
}

void StaticPolygon::calculate_orientation() {
    float total = 0;
    for (size_t i = 0; i < bridged_contour.vertices.size(); ++i) {
        const auto p0 = bridged_contour.vertices[i];
        const auto p1 = bridged_contour.vertices[(i + 1) % bridged_contour.vertices.size()];
        total += (p1.x - p0.x) * (p1.y + p0.y);
    }
    orientation = total > 0 ? ContourOrientation::CounterClockwise : ContourOrientation::Clockwise;
}

void StaticPolygon::generate_bridged_contour() {
    if (outer_contour.vertices.size() <= 2)
    {
        Warnings::warn(std::format("Warning: your polygon has only {} vertices", outer_contour.vertices.size()), Warnings::Type::PolygonVertexMismatch);
    }
    bridged_contour = outer_contour;
    for (const auto & contour : inner_polygon_contours) {
        // search
        const auto start = contour.get_rightmost();
        auto ray = Line(start.x, start.y, outer_contour.get_rightmost().x, start.y);
        std::optional<Vec2> nearest_intersection = std::nullopt;
        size_t nearest_intersection_vertex_index = 0;
        for (size_t l_i = 0; l_i < bridged_contour.vertices.size(); ++l_i) {
            const auto& p0 = bridged_contour.vertices[l_i];
            const auto& p1 = bridged_contour.vertices[(l_i + 1) % bridged_contour.vertices.size()];
            Line line(p0.x, p0.y, p1.x, p1.y);
            if (const auto& [is_intersecting, intersection_type, intersection] = ray.intersection(line); is_intersecting && (!nearest_intersection || intersection.x < nearest_intersection.value().x)) {
                nearest_intersection = intersection;
                nearest_intersection_vertex_index = l_i;
            }
        }
        if (!nearest_intersection)
            THROW_RUNTIME("Inner contour is invalid!")
        // insertion
        std::vector<Vec2> joined_vertices(bridged_contour.vertices.size() + contour.vertices.size() + 3);
        memcpy(joined_vertices.data(), bridged_contour.vertices.data(), nearest_intersection_vertex_index * sizeof(Vec2)); // everything until intersection but without the intersection point - search marked the intersection's point, which is located before the second point of the intersected line in the array
        const auto& p0 = bridged_contour.vertices[nearest_intersection_vertex_index];
        joined_vertices[nearest_intersection_vertex_index] = p0; // add the first point of the intersected line
        joined_vertices[nearest_intersection_vertex_index + 1] = nearest_intersection.value(); // add intersection point -> this is the first additional geometry point
        const size_t bridge_to_index = contour.vertices.size() - contour.rightmost_index;
        if (contour.get_orientation() != outer_contour.get_orientation()) {
            memcpy(joined_vertices.data() + nearest_intersection_vertex_index + 2, contour.vertices.data() + contour.rightmost_index, bridge_to_index* sizeof(Vec2)); // copy the first unrotated half and rotate otf
            memcpy(joined_vertices.data() + bridge_to_index + nearest_intersection_vertex_index + 2, contour.vertices.data(), contour.rightmost_index * sizeof(Vec2)); // copy the second unrotated half and rotate otf
        }
        else {
            for (size_t i = 0; i < contour.vertices.size(); ++i) {
                const size_t index =
                    (contour.rightmost_index - i + contour.vertices.size())
                    % contour.vertices.size();

                joined_vertices[
                    nearest_intersection_vertex_index + 2 + i
                ] = contour.vertices[index];
            }
        }
        const size_t bridge_back_index = nearest_intersection_vertex_index + 2 + contour.vertices.size();
        joined_vertices[bridge_back_index] = contour.vertices[contour.rightmost_index]; // add bridge point of the inner contour for the back bridge -> this is the second additional geometry point
        joined_vertices[bridge_back_index + 1] = nearest_intersection.value(); // add intersection point -> this is the third additional geometry point
        // what happens if the intersection is on the rotation boundary of the outer contour?
        // everything was copied until the first point, including start point - which is the second point of the intersected line: no geometrical problem
        memcpy(joined_vertices.data() + bridge_back_index + 2, bridged_contour.vertices.data() + nearest_intersection_vertex_index + 1, (bridged_contour.vertices.size() - nearest_intersection_vertex_index - 1) * sizeof(Vec2)); // copy the rest of the outer contour
        bridged_contour.vertices = joined_vertices;
    }
}

void StaticPolygon::sort_polygon_contours() {
    std::ranges::stable_sort(inner_polygon_contours,
         [](const InnerPolygonContour& a, const InnerPolygonContour& b) {
             return a.get_rightmost().x > b.get_rightmost().x;
         }
    );
}
