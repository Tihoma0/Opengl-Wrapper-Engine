#ifndef RECTANGLEGROUP_H
#define RECTANGLEGROUP_H
#include <memory>
#include <vector>

#include "Rectangle.h"
#include "rendercore/mesh/Mesh.h"


class RectangleGroup {
public:
    RectangleGroup(const std::vector<Rectangle> &rectangles, const std::vector<Color> &colors);

    void build_mesh();

    void add(const Rectangle &rectangle, const Color &color);

    Rectangle& get_rectangle(int index);

    Color& get_color(int index);

    void set(int i, const Rectangle &rectangle, const Color &color);
    void set(int i, const Color &color);
    void set(int i, const Rectangle &rectangle);

    void clear();

    void remove(int index);

    std::shared_ptr<Mesh> get_mesh() { return mesh; }



private:
    std::vector<Rectangle> rects;
    std::vector<Color> colors;
    std::shared_ptr<Mesh> mesh;
    bool is_dirty = true;
};



#endif //RECTANGLEGROUP_H
