#pragma once

#include <Eigen/Core>

class Box2d
{
  public:
    Box2d() = delete;
    Box2d(Box2d &box) : _a(box._a), _b(box._b) {}
    Box2d(Eigen::Vector2d a, Eigen::Vector2d b)
        : _a(std::min(a.x, b.x), std::min(a.y, b.y)), _b(std::max(a.x, b.x), std::max(a.y, b.y)) {}
    Box2d(double ax, double ay, double bx, double by)
        : _a(std::min(ax, bx), std::min(ay, by)), _b(std::max(ax, bx), std::max(ay, by)) {}

    bool contains(const Eigen::Vector2d p) const;
    bool intersect(const Box2d &b) const;

  private:
    Eigen::Vector2d _a;
    Eigen::Vector2d _b;
};