#ifndef BEZIERGROUP_H
#define BEZIERGROUP_H
#include <variant>
#include <vector>

#include "BezierCurve.h"

using Bezier = std::variant<QuadraticBezierCurve, CubicBezierCurve>;
class BezierGroup {
public:
    explicit BezierGroup(const std::vector<Bezier> &curves = {}, const std::vector<Color> &colors = {});

    void add_curve(const Bezier &curve, const Color &color);

    void build_cubic_mesh();
    void build_quadratic_mesh();

    std::shared_ptr<Mesh> get_cubic_mesh();
    std::shared_ptr<Mesh> get_quadratic_mesh();

private:
    std::vector<CubicBezierCurve> cubic_curves;
    std::vector<QuadraticBezierCurve> quadratic_curves;
    std::shared_ptr<Mesh> cubic_mesh;
    std::shared_ptr<Mesh> quadratic_mesh;
    bool is_quadratic_dirty = true;
    bool is_cubic_dirty = true;
    std::vector<Color> quadratic_colors;
    std::vector<Color> cubic_colors;
};



#endif //BEZIERGROUP_H
