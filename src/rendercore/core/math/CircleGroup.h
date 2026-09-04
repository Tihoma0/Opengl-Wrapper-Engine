#ifndef CIRCLEGROUP_H
#define CIRCLEGROUP_H
#include <memory>
#include <vector>

#include "rendercore/mesh/Mesh.h"

class Color;
class Circle;



class CircleGroup {
public:
    explicit CircleGroup(const std::vector<Circle> &circles);

    void add_circle(const Circle &circle, const Color &color);

    void build_mesh();

    std::shared_ptr<Mesh> get_mesh() { return mesh; }
private:
    bool is_dirty = true;
    std::vector<Circle> circles;
    std::vector<Color> colors;
    std::shared_ptr<Mesh> mesh;
};



#endif //CIRCLEGROUP_H
