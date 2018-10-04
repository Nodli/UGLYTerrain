#pragma once

#include <Grid2d.hpp>

#include <vector>

class ScalarField : public Grid2d
{
  public:
    ScalarField() = delete;
    ScalarField(const ScalarField& hf)
        : Grid2d(hf), _heights(hf._heights) {_heights.resize(hf.cell_number());}
    ScalarField(const Grid2d &g)
        : Grid2d(g) {_heights.resize(g.cell_number());}
    ScalarField(const Box2d &b, const int width, const int height)
        : Grid2d(b, width, height) {_heights.resize(width*height);}
    ScalarField(const Eigen::Vector2d a, const Eigen::Vector2d b, const int width, const int height)
        : Grid2d(a, b, width, height) {_heights.resize(width*height);}

    int index(const int i, const int j) const {return _grid_width*i+j;}

    double get_height(const int i, const int j) const {return _heights.at(index(i, j));}
    double get_height(const double x, const double y) const;

    void set_height(const int i, const int j, double value);

  private:
    std::vector<double> _heights;
};
