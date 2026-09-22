
#ifndef GLWRAPPER_II_POLYGON_H
#define GLWRAPPER_II_POLYGON_H
#include <memory>
#include <vector>

#include "Line.h"


class Mesh;

struct PolygonContour {
    std::vector<Line> lines = {};
};


class Polygon {
public:
    Polygon(const std::vector<PolygonContour> &contours = {});
     
    void build_mesh();
    std::shared_ptr<Mesh> get_mesh();
private:
    std::vector<PolygonContour> contours;
    std::shared_ptr<Mesh> mesh;
};



#endif //GLWRAPPER_II_POLYGON_H
