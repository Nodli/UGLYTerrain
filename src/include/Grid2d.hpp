#pragma once

#include <Box2d.hpp>

class Grid2d : public Box2d
{
  public:
    Grid2d() = delete;
    Grid2d(const Grid2d &g);
    Grid2d(Grid2d &&g);
    Grid2d(const Box2d &b, const int width, const int height);
    Grid2d(const int width, const int height, const Eigen::Vector2d a = {0, 0}, const Eigen::Vector2d b = {1, 1});

    int grid_width() const {return _grid_width;}
    int grid_height() const {return _grid_height;}

    int cell_number() const { return _grid_width * _grid_height; }
    Eigen::Vector2d position(const int x, const int y) const;
    Eigen::Vector2i grid_position(const double x, const double y) const;

    void reshape(double ax, double ay, double bx, double by);
    void reshape(const Eigen::Vector2d a, const Eigen::Vector2d b);

    Grid2d& operator=(const Grid2d& g);
    Grid2d& operator=(Grid2d&& g);

  protected:
    int _grid_width;
    int _grid_height;
    Eigen::Vector2d _cell_size;
};
