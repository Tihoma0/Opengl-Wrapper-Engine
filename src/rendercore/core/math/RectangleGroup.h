#ifndef RECTANGLEGROUP_H
#define RECTANGLEGROUP_H
#include <iostream>
#include <memory>
#include <vector>

#include "Rectangle.h"
#include "rendercore/mesh/Mesh.h"


class RectangleGroup {
public:
    RectangleGroup(const std::vector<Rectangle> &rectangles);


    void build_mesh();

    void add_rectangle(const Rectangle &rectangle);

    std::shared_ptr<Mesh> get_mesh() { return mesh; }

private:
    std::vector<Rectangle> rects;
    std::shared_ptr<Mesh> mesh;
    bool is_dirty = true;
};



#endif //RECTANGLEGROUP_H
