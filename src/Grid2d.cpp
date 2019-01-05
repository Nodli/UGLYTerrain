#include <Grid2d.hpp>
#include <cassert>

const int Grid2d::def_nei[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}};
const double Grid2d::def_nei_dist[] = {sqrt(2.), 1., sqrt(2.), 1., 1., sqrt(2.), 1., sqrt(2.)};

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

Grid2d::Grid2d(const int width, const int height, const Eigen::Vector2d a, const Eigen::Vector2d b)
	: Box2d(a, b), _grid_width(width), _grid_height(height)
{
	_cell_size = Eigen::Vector2d(this->width() / (_grid_width - 1), this->height() / (_grid_height - 1));
}

int Grid2d::neighbors(const int i, const int j, Eigen::Vector2i* p) const
{
	assert(p != nullptr);

	int nb = 0;

	for(int k = 0; k < 8; ++k)
	{
		if(def_nei[k][0] + i >= 0 && def_nei[k][0] + i < _grid_width
				&& def_nei[k][1] + j >= 0 && def_nei[k][1] + j < _grid_height)
		{
			p[nb++] = Eigen::Vector2i(def_nei[k][0] + i, def_nei[k][1] + j);
		}
	}

	return nb;
}

Eigen::Vector2d Grid2d::position(const int x, const int y) const
{
	return min() + Eigen::Vector2d(x * _cell_size(0), y * _cell_size(1));
}

Eigen::Vector2i Grid2d::grid_position(const double x, const double y) const
{
	int dx = std::floor(((x - _a(0)) / (_b(0) - _a(0))) * (_grid_width - 1));
	int dy = std::floor(((y - _a(1)) / (_b(1) - _a(1))) * (_grid_height - 1));
	return Eigen::Vector2i(dx, dy);
}

Eigen::Vector2i Grid2d::grid_position(const Eigen::Vector2d p) const
{
	return grid_position(p(0), p(1));
}

Grid2d &Grid2d::operator=(const Grid2d &g)
{
	Box2d::operator=(g);
	this->_grid_width = g._grid_width;
	this->_grid_height = g._grid_height;
	this->_cell_size = g._cell_size;
	return *this;
}

Grid2d &Grid2d::operator=(Grid2d &&g)
{
	Box2d::operator=(std::move(g));
	*this = g;
	return *this;
}
