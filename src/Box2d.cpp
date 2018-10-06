#include "Box2d.hpp"

bool Box2d::contains(const Eigen::Vector2d p) const
{
	return p(0) > _a(0) && p(0) < _b(0) && p(1) > _a(1) && p(1) < _b(1);
}

bool Box2d::intersect(const Box2d &b) const
{
	if (_b(0) < b._a(0) || _a(0) > b._b(0))
		return false;

	if (_b(1) < b._a(1) || _a(1) > b._b(1))
		return false;

	return true;
}

void Box2d::reshape(double ax, double ay, double bx, double by)
{
	_a = {std::min(ax, bx), std::min(ay, by)};
	_b = {std::max(ax, bx), std::max(ay, by)};
}

void Box2d::reshape(const Eigen::Vector2d a, const Eigen::Vector2d b)
{
	_a = {std::min(a(0), b(0)), std::min(a(1), b(1))};
	_b = {std::max(a(0), b(0)), std::max(a(1), b(1))};
}

Box2d &Box2d::operator=(const Box2d &b)
{
	this->_a = b._a;
	this->_b = b._b;

	return *this;
}

Box2d &Box2d::operator=(Box2d &&b)
{
	*this = b;
	return *this;
}
