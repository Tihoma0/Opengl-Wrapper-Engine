
#ifndef GLWRAPPER_II_POLYGON_H
#define GLWRAPPER_II_POLYGON_H
#include <memory>
#include <vector>

#include "Line.h"


class Mesh;

struct OuterPolygonContour {
    std::vector<Line> lines = {};
};

struct InnerPolygonContour {
    std::vector<Line> lines = {};
};


class Polygon {
public:
    Polygon(const OuterPolygonContour &outer_contours,
    const std::vector<InnerPolygonContour> &inner_polygon_contours) :
    outer_contours(outer_contours),
    inner_polygon_contours(inner_polygon_contours) {}

    void build_mesh();
    std::shared_ptr<Mesh> get_mesh();
private:
    const OuterPolygonContour& outer_contours;
    const std::vector<InnerPolygonContour>& inner_polygon_contours;
    std::shared_ptr<Mesh> mesh;
    OuterPolygonContour generate_bridged_contour();
};



#endif //GLWRAPPER_II_POLYGON_H
