#pragma once

#include <Box2d.hpp>

class Grid2d : public Box2d
{
  public:
    Grid2d() = delete;
    Grid2d(const Grid2d &g);
    Grid2d(const Box2d &b, const int width, const int height);
    Grid2d(const Eigen::Vector2d a, const Eigen::Vector2d b, const int width, const int height);

    int cell_number() const { return _grid_width * _grid_height; }
    Eigen::Vector2d position(const int x, const int y) const;
    Eigen::Vector2i grid_position(const double x, const double y) const;

  protected:
    const int _grid_width;
    const int _grid_height;
    Eigen::Vector2d _cell_size;
};
