
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

    LineGroup();

    void add(const Line &line, const Color &color);

    void build_line_mesh();

    void build_quad_mesh();

    Line& get_line(int index);

    Color& get_color(int index);

    void set(int i, const Line &line, const Color &color);
    void set(int i, const Color &color);
    void set(int i, const Line &line);

    void clear();

    void remove(int index);

    std::shared_ptr<Mesh> get_line_mesh() { return line_mesh; }

    std::shared_ptr<Mesh> get_quad_mesh() { return quad_mesh; }
private:
    bool is_line_dirty = true;
    bool is_quad_dirty = true;
    std::vector<Line> lines;
    std::vector<Color> colors;
    std::shared_ptr<Mesh> line_mesh;
    std::shared_ptr<Mesh> quad_mesh;
};



#endif //LineGroup_H
