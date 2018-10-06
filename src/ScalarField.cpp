#include <ScalarField.hpp>

double ScalarField::get_value(const double x, const double y) const
{
    Eigen::Vector2i ij = grid_position(x, y);
    Eigen::Vector2d pij = position(ij(0), ij(1));
    Eigen::Vector2d diff(x, y);
    diff -= pij;
    double u = diff(0) / _cell_size(0);
    double v = diff(1) / _cell_size(1);
    return (1 - u) * (1 - v) * get_value(ij(0), ij(1)) + (1 - u) * v * get_value(ij(0), ij(1) + 1) + u * (1 - v) * get_value(ij(0) + 1, ij(1)) + u * v * get_value(ij(0) + 1, ij(1) + 1);
}

Eigen::Vector2d ScalarField::gradient(int i, int j) const
{
    Eigen::Vector2d grad;
    double delta_x = width() / _grid_width;
    double delta_y = height() / _grid_height;
    double scale = 1;

    if (i <= 0)
    {
        grad(0) = (_values.at(index(i + 1, j)) - _values.at(index(i, j))) / delta_x;
    }
    else if (i >= _grid_width - 1)
    {
        grad(0) = (_values.at(index(i, j)) - _values.at(index(i - 1, j))) / delta_x;
    }
    else
    {
        grad(0) = (_values.at(index(i + 1, j)) - _values.at(index(i - 1, j))) / (2.0 * delta_x);
    }

    if (j <= 0)
    {
        grad(1) = (_values.at(index(i, j + 1)) - _values.at(index(i, j))) / delta_y;
    }
    else if (j >= _grid_height - 1)
    {
        grad(1) = (_values.at(index(i, j)) - _values.at(index(i, j - 1))) / delta_y;
    }
    else
    {
        grad(1) = (_values.at(index(i, j + 1)) - _values.at(index(i, j - 1))) / (2.0 * delta_y);
    }

    return scale * grad;
}

double ScalarField::slope(int i, int j) const
{
    Eigen::Vector2d grad = gradient(i, j);
    return sqrt(grad.dot(grad));
}

void ScalarField::set_value(const int i, const int j, double value)
{
    _values.at(index(i, j)) = value;
}

ScalarField& ScalarField::operator=(const ScalarField& sf)
{
    if(this != &sf)
    {
        Grid2d::operator=(sf);
        this->_values = sf._values;
    }

    return *this;
}

ScalarField& ScalarField::operator=(ScalarField&& sf)
{
    if(this != &sf)
    {
        Grid2d::operator=(sf);
        this->_values = std::move(sf._values);
    }

    return *this;
}