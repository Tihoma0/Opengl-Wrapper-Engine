//
// Created by marku on 03.09.2026.
//

#ifndef CIRCLEGROUP_H
#define CIRCLEGROUP_H
#include <memory>
#include <vector>

#include "rendercore/mesh/Mesh.h"

class Circle;



class CircleGroup {
public:
    explicit CircleGroup(const std::vector<Circle> &circles);

    void add_circle(const Circle &circle);

    void build_mesh();

    std::shared_ptr<Mesh> get_mesh() { return mesh; }
private:
    bool is_dirty = true;
    std::vector<Circle> circles;
    std::shared_ptr<Mesh> mesh;
};



#endif //CIRCLEGROUP_H
