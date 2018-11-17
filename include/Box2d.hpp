#pragma once

#include <Eigen/Core>

#include <cmath>

/**
 * @brief Defines a 2D box as two 2D points
 *
 */
class Box2d
{
public:
	Box2d() = delete;
	/**
	 * @brief Construct a new Box 2d object from an other box
	 *
	 * @param box the box to copy
	 */
	Box2d(const Box2d &box) : _a(box._a), _b(box._b) {}
	/**
	 * @brief Construct a new Box 2d object from an other box
	 *
	 * @param box the box to copy
	 */
	Box2d(Box2d &&box) : _a(box._a), _b(box._b) {}
	/**
	 * @brief Construct a new Box 2d object from two points
	 *
	 * @param a first point of the box
	 * @param b second point of the box
	 */
	Box2d(const Eigen::Vector2d a, const Eigen::Vector2d b)
		: _a(std::min(a(0), b(0)), std::min(a(1), b(1))), _b(std::max(a(0), b(0)), std::max(a(1), b(1))) {}
	/**
	 * @brief Construct a new Box 2d object from two positions
	 *
	 * @param ax first x position
	 * @param ay first y position
	 * @param bx second x position
	 * @param by second y position
	 */
	Box2d(double ax, double ay, double bx, double by)
		: _a(std::min(ax, bx), std::min(ay, by)), _b(std::max(ax, bx), std::max(ay, by)) {}

	/**
	 * @brief Gives the width of the box
	 *
	 * @return double the width of the box
	 */
	double width() const
	{
		return _b(0) - _a(0);
	}

	/**
	 * @brief Gives the height of the box
	 *
	 * @return double the height of the box
	 */
	double height() const
	{
		return _b(1) - _a(1);
	}

	/**
	 * @brief Gives the lower left corner of the box
	 *
	 * @return Eigen::Vector2d the lower left corner of the box
	 */
	Eigen::Vector2d min() const
	{
		return _a;
	}

	/**
	 * @brief Tells if a point is inside a box
	 *
	 * @param p 2D point
	 * @return true if the point is inside the box
	 * @return false if the point is not inside the box
	 */
	bool contains(const Eigen::Vector2d p) const;
	/**
	 * @brief Tells if a box intersect this one
	 *
	 * @param b box to check intersection with
	 * @return true if b intersect this box
	 * @return false if b does not intersect this box
	 */
	bool intersect(const Box2d &b) const;

	/**
	 * @brief Reshape the box to a new size and position
	 *
	 * @param ax first x position
	 * @param ay first y position
	 * @param bx second x position
	 * @param by second y position
	 */
	void reshape(double ax, double ay, double bx, double by);
	/**
	 * @brief Reshape the box to a nex size and position
	 *
	 * @param a first point of the box
	 * @param b second point of the box
	 */
	void reshape(const Eigen::Vector2d a, const Eigen::Vector2d b);

	/**
	 * @brief Affectation operator
	 *
	 * @param b the box used for the affectation
	 * @return Box2d& a reference to this box
	 */
	Box2d &operator=(const Box2d &b);
	/**
	 * @brief Rvalue affectation operator
	 *
	 * @param b the box used for the affectation
	 * @return Box2d& a reference to this box
	 */
	Box2d &operator=(Box2d &&b);

protected:
	Eigen::Vector2d _a; /**< Lower left corner of the box*/
	Eigen::Vector2d _b; /**< Upper right corner of the box*/
};
