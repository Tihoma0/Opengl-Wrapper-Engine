//
// Created by marku on 03.09.2026.
//

#ifndef LINEGROUP_H
#define LINEGROUP_H
#include <memory>
#include <vector>

#include "rendercore/mesh/Mesh.h"

class Color;
class Line;



class LineGroup {
public:
    explicit LineGroup(const std::vector<Line> &lines, const std::vector<Color> &colors);

    void add(const Line &line, const Color &color);

    void build_mesh();

    Line& get_line(int index);

    Color& get_color(int index);

    void set(int i, const Line &line, const Color &color);
    void set(int i, const Color &color);
    void set(int i, const Line &line);

    void clear();

    void remove(int index);

    std::shared_ptr<Mesh> get_mesh() { return mesh; }
private:
    bool is_dirty = true;
    std::vector<Line> lines;
    std::vector<Color> colors;
    std::shared_ptr<Mesh> mesh;
};



#endif //LineGroup_H
