#include <Grid2d.hpp>

Grid2d::Grid2d(const Grid2d &g)
    : Box2d(g), _grid_width(g._grid_width), _grid_height(g._grid_height), _cell_size(g._cell_size)
{
}

Grid2d::Grid2d(Grid2d &&g)
    : Box2d(std::move(g)), _grid_width(g._grid_width), _grid_height(g._grid_height), _cell_size(g._cell_size)
{
}

Grid2d::Grid2d(const Box2d &b, const int width, const int height)
    : Box2d(b), _grid_width(width), _grid_height(height)
{
    _cell_size = Eigen::Vector2d(this->width() / (_grid_width - 1), this->height() / (_grid_height - 1));
}

Grid2d::Grid2d(const Eigen::Vector2d a, const Eigen::Vector2d b, const int width, const int height)
    : Box2d(a, b), _grid_width(width), _grid_height(height)
{
    _cell_size = Eigen::Vector2d(this->width() / (_grid_width - 1), this->height() / (_grid_height - 1));
}

Eigen::Vector2d Grid2d::position(const int x, const int y) const
{
    return min() + Eigen::Vector2d(x * _cell_size(0), y * _cell_size(1));
}

Eigen::Vector2i Grid2d::grid_position(const double x, const double y) const
{
    int dx = std::floor((x - _a(0) / _b(0) - _a(0)) * (_grid_width - 1));
    int dy = std::floor((y - _a(1) / _b(1) - _a(1)) * (_grid_height - 1));
    return Eigen::Vector2i(dx, dy);
}

Grid2d& Grid2d::operator=(const Grid2d& g)
{
    Box2d::operator=(g);
    this->_grid_width = g._grid_width;
    this->_grid_height = g._grid_height;
    this->_cell_size = g._cell_size;
    return *this;
}

Grid2d& Grid2d::operator=(Grid2d&& g)
{
    Box2d::operator=(std::move(g));
    *this = g;
    return *this;
}