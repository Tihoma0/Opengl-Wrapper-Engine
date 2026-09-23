
#include "Polygon.h"

#include "rendercore/mesh/Mesh.h"




void Polygon::build_mesh() {
    mesh = Mesh::create();
    std::vector<Line> lines;
}

std::shared_ptr<Mesh> Polygon::get_mesh() {
    build_mesh();
    return mesh;
}

OuterPolygonContour Polygon::generate_bridged_contour() {
    OuterPolygonContour contour;
    for (auto& contour : inner_polygon_contours) {
        
    }
}
