//
// Created by marku on 03.09.2026.
//

#ifndef CIRCLEGROUP_H
#define CIRCLEGROUP_H
#include <memory>
#include <vector>

#include "rendercore/mesh/Mesh.h"

class Color;
class Circle;



class CircleGroup {
public:
    explicit CircleGroup(const std::vector<Circle> &circles, const std::vector<Color> &colors);

    void add(const Circle &circle, const Color &color);

    void build_mesh();

    Circle& get_circle(int index);

    Color& get_color(int index);

    void set(int i, const Circle &circle, const Color &color);
    void set(int i, const Color &color);
    void set(int i, const Circle &circle);

    void clear();

    void remove(int index);

    std::shared_ptr<Mesh> get_mesh() { return mesh; }
private:
    bool is_dirty = true;
    std::vector<Circle> circles;
    std::vector<Color> colors;
    std::shared_ptr<Mesh> mesh;
};



#endif //CIRCLEGROUP_H
