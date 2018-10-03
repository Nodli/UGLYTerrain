#include "Box2d.hpp"

bool Box2d::contains(const Eigen::Vector2d p) const
{
	return p(0) > _a(0) && p(0) < _b(0) && p(1) > _a(1) && p(1) < _b(1);
}

bool Box2d::intersect(const Box2d &b) const
{
	if(_b(0) < b._a(0) || _a(0) > b._b(0))
		return false;

	if(_b(1) < b._a(1) || _a(1) > b._b(1))
		return false;

	return true;
}
