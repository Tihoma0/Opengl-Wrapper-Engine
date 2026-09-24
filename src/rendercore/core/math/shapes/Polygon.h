
#ifndef GLWRAPPER_II_POLYGON_H
#define GLWRAPPER_II_POLYGON_H
#include <memory>
#include <vector>

#include "Line.h"


class Mesh;

class OuterPolygonContour {
public:
    std::vector<Vec2> vertices = {};
    Vec2 get_rightmost() const;
    mutable size_t rightmost_index = -1;
private:
    mutable Vec2 rightmost = {0, 0};
    mutable bool has_rightmost = false;
};

class InnerPolygonContour {
public:
    std::vector<Vec2> vertices = {};
    Vec2 get_rightmost() const;
    mutable size_t rightmost_index = -1;
private:
    mutable Vec2 rightmost = {0, 0};
    mutable bool has_rightmost = false;
};


class Polygon {
public:
    Polygon(const OuterPolygonContour &outer_contour,
    std::vector<InnerPolygonContour>& inner_polygon_contours) :
    outer_contour(outer_contour),
    inner_polygon_contours(std::move(inner_polygon_contours)) {}

    void build_mesh();
    std::shared_ptr<Mesh> get_mesh();
private:
    const OuterPolygonContour& outer_contour;
    std::vector<InnerPolygonContour> inner_polygon_contours;
    std::shared_ptr<Mesh> mesh;
    OuterPolygonContour generate_bridged_contour();
    void sort_polygon_contours();
};



#endif //GLWRAPPER_II_POLYGON_H
