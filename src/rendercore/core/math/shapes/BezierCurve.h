#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H
#include <memory>
#include <vector>

#include "rendercore/core/math/vec/Vec2.h"

class Mesh;

class BezierCurve {
public:
    virtual ~BezierCurve() = default;

    [[nodiscard]] virtual float get_width() const = 0;

    [[nodiscard]] virtual Vec2 evaluate(float t) const = 0;

    [[nodiscard]] virtual std::vector<Vec2> get_control_points() const = 0;

    [[nodiscard]] virtual std::shared_ptr<Mesh> get_mesh() = 0;

    virtual void build_mesh() = 0;
};

class CubicBezierCurve final : public BezierCurve {
public:
    float width;
    Vec2 p1, p2, p3, p4;
    std::shared_ptr<Mesh> mesh;

    CubicBezierCurve(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4, const float width = 1.0f) : width(width), p1(p1), p2(p2), p3(p3), p4(p4) {}

    [[nodiscard]] Vec2 evaluate(float t) const override;

    [[nodiscard]] float get_width() const override;

    [[nodiscard]] std::vector<Vec2> get_control_points() const override;

    [[nodiscard]] std::shared_ptr<Mesh> get_mesh() override;

    void build_mesh() override;
};

class QuadraticBezierCurve final : public BezierCurve {
public:
    float width;
    Vec2 p1, p2, p3;
    std::shared_ptr<Mesh> mesh;

    QuadraticBezierCurve(const Vec2& p1, const Vec2& p2, const Vec2& p3, const float width = 1.0f) : width(width), p1(p1), p2(p2), p3(p3) {}

    [[nodiscard]] Vec2 evaluate(float t) const override;

    [[nodiscard]] float get_width() const override;

    [[nodiscard]] std::vector<Vec2> get_control_points() const override;

    [[nodiscard]] std::shared_ptr<Mesh> get_mesh() override;

    void build_mesh() override;
};




#endif //BEZIERCURVE_H
