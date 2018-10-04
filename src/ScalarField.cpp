#include <ScalarField.hpp>

double ScalarField::get_height(const double x, const double y) const
{
    Eigen::Vector2i ij = grid_position(x, y);
    Eigen::Vector2d pij = position(ij(0), ij(1));
    Eigen::Vector2d diff(x, y);
    diff -= pij;
    double u = diff(0) / _cell_size(0);
    double v = diff(1) / _cell_size(1);
    return (1 - u) * (1 - v) * get_height(ij(0), ij(1))
    + (1 - u) * v * get_height(ij(0), ij(1) + 1) 
    + u * (1 - v) * get_height(ij(0) + 1, ij(1)) 
    + u * v * get_height(ij(0) + 1, ij(1) + 1);
}

void ScalarField::set_height(const int i, const int j, double value)
{
    _heights.at(index(i, j)) = value;
}