#include <Grid2d.hpp>

Eigen::Vector2d Grid2d::position(const int x, const int y) const
{
    double x_scale = _box.width() / (_width-1);
    double y_scale = _box.height() / (_height-1);
    return _box.min() + Eigen::Vector2d(x * x_scale, y * y_scale);
}