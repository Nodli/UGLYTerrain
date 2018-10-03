#pragma once

#include <Box2d.hpp>

class Grid2d
{
  public:
    Grid2d() = delete;
    Grid2d(const Grid2d &g)
        : _box(g._box), _width(g._width), _height(g._height) {}
    Grid2d(const Box2d &b, const int width, const int height)
        : _box(b), _width(width), _height(height) {}
    Grid2d(const Eigen::Vector2d a, const Eigen::Vector2d b, const int width, const int height)
        : _box(a, b), _width(width), _height(height) {}

    Eigen::Vector2d position(const int x, const int y) const;

  private:
    const Box2d _box;
    const int _width;
    const int _height;
};
