#include "Box2d.hpp"

bool Box2d::contains(const Eigen::Vector2d p) const
{
	return p.x > _a.x && p.x < _b.x && p.y > _a.y && p.y < _b.y;
}

bool Box2d::intersect(const Box2d &b) const
{
	if(_b.x < b._a.x || _a.x > b._b.x)
		return false;

	if(_b.y < b._a.y || _a.y > b._b.y)
		return false;

	return true;
}