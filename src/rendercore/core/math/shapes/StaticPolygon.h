
#ifndef GLWRAPPER_II_POLYGON_H
#define GLWRAPPER_II_POLYGON_H
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "Line.h"


class Mesh;

enum class ContourOrientation {
    Clockwise,
    CounterClockwise
};

class OuterPolygonContour {
public:
    OuterPolygonContour(const std::initializer_list<Vec2> vertices)
        : vertices(vertices) {}
    std::vector<Vec2> vertices;
    Vec2 get_rightmost() const;
    ContourOrientation get_orientation() const;
    mutable size_t rightmost_index = -1;
private:
    mutable Vec2 rightmost = {0, 0};
    mutable bool has_rightmost = false;
    mutable ContourOrientation orientation = ContourOrientation::Clockwise;
    mutable bool has_orientation = false;
};

class InnerPolygonContour {
public:
    InnerPolygonContour(const std::initializer_list<Vec2> vertices)
        : vertices(vertices) {}
    std::vector<Vec2> vertices;
    Vec2 get_rightmost() const;
    ContourOrientation get_orientation() const;
    mutable size_t rightmost_index = -1;
private:
    mutable Vec2 rightmost = {0, 0};
    mutable bool has_rightmost = false;
    mutable ContourOrientation orientation = ContourOrientation::Clockwise;
    mutable bool has_orientation = false;
};


class StaticPolygon {
public:
    const OuterPolygonContour outer_contour;
    std::vector<InnerPolygonContour> inner_polygon_contours;

    StaticPolygon(OuterPolygonContour outer_contour, const std::vector<InnerPolygonContour>& inner_polygon_contours) : outer_contour(std::move(outer_contour)), inner_polygon_contours(inner_polygon_contours) {
        sort_polygon_contours();
        calculate_orientation();
        generate_bridged_contour();

    }

    void build_mesh();
    [[nodiscard]] OuterPolygonContour get_bridged_contour() const { return bridged_contour; }
    ContourOrientation get_orientation() const { return orientation; };
    std::shared_ptr<Mesh> get_mesh();
private:
    std::shared_ptr<Mesh> mesh;
    OuterPolygonContour bridged_contour = {};

    ContourOrientation orientation;

    void calculate_orientation();
    void generate_bridged_contour();
    void sort_polygon_contours();
};



#endif //GLWRAPPER_II_POLYGON_H
