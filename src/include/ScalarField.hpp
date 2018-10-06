#pragma once

#include <Grid2d.hpp>

#include <vector>

class ScalarField : public Grid2d
{
  public:
    ScalarField() = delete;
    ScalarField(const ScalarField& hf)
        : Grid2d(hf), _values(hf._values) {}
    ScalarField(ScalarField&& hf)
        : Grid2d(hf), _values(std::move(hf._values)) {}
    ScalarField(const Grid2d &g)
        : Grid2d(g) {_values.resize(g.cell_number());}
    ScalarField(const Box2d &b, const int width, const int height)
        : Grid2d(b, width, height) {_values.resize(width*height);}
    ScalarField(const int width, const int height, const Eigen::Vector2d a = {0, 0}, const Eigen::Vector2d b = {1, 1})
        : Grid2d(width, height, a, b) {_values.resize(width*height);}

    int index(const int i, const int j) const {return _grid_width*i+j;}

    double get_value(const int i, const int j) const {return _values.at(index(i, j));}
    double get_value(const double x, const double y) const;

    Eigen::Vector2d gradient(int i, int j) const;
    double slope(int i, int j) const;

    void set_value(const int i, const int j, double value);
    double& at(const int i, const int j) {return _values.at(index(i, j));}
    void copy_values(const ScalarField& sf);
    void copy_values(ScalarField&& sf);

    ScalarField& operator=(const ScalarField& sf);
    ScalarField& operator=(ScalarField&& sf);

  private:
    std::vector<double> _values;
};
