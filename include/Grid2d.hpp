#pragma once

#include <Box2d.hpp>

/**
 * @brief Defines a 2D grid as a subdivided 2D box
 *
 */
class Grid2d : public Box2d
{
public:
	Grid2d() = delete;
	/**
	 * @brief Construct a new Grid 2d object from an other grid
	 *
	 * @param g the grid to copy
	 */
	Grid2d(const Grid2d &g);
	/**
	 * @brief Construct a new Grid 2d object from an other grid
	 *
	 * @param g the grid to copy
	 */
	Grid2d(Grid2d &&g);
	/**
	 * @brief Construct a new Grid 2d object from a box
	 *
	 * @param b         the box in wich to create the grid
	 * @param width     the number of cells along the width of the grid
	 * @param height    the number of cells along the height of the grid
	 */
	Grid2d(const Box2d &b, const int width, const int height);
	/**
	 * @brief Construct a new Grid 2d object from scratch
	 *
	 * @param width     the number of cells along the width of the grid
	 * @param height    the number of cells along the height of the grid
	 * @param a         the first point of the grid
	 * @param b         the second point of the grid
	 */
	Grid2d(const int width, const int height, const Eigen::Vector2d a = {0, 0}, const Eigen::Vector2d b = {1, 1});

	/**
	 * @brief Gets the number of cells along the width of the grid
	 *
	 * @return int      the width of the grid
	 */
	int grid_width() const
	{
		return _grid_width;
	}
	/**
	 * @brief Gets the number of cells along the height of the grid
	 *
	 * @return int      the height of the grid
	 */
	int grid_height() const
	{
		return _grid_height;
	}

	/**
	 * @brief Gets the total number of cells of the grid
	 *
	 * @return int      the total number of cells
	 */
	int cell_number() const
	{
		return _grid_width * _grid_height;
	}
	/**
	 * @brief Gets the position of a cell.
	 *
	 * @param x         the position of the cell on the width
	 * @param y         the position of the cell on the height
	 * @return Eigen::Vector2d  the position of the lower left corner of the cell in the 2d plan
	 */
	Eigen::Vector2d position(const int x, const int y) const;
	/**
	 * @brief Gets the position on the grid of a point in the plan
	 *
	 * @param x, y      the position of point on the plane
	 *
	 * @return Eigen::Vector2i the position of the corresponding cell in the grid
	 */
	Eigen::Vector2i grid_position(const double x, const double y) const;
	/**
	 * @brief Gets the position on the grid of a point in the plan
	 *
	 * @param p         the position of point on the plane
	 *
	 * @return Eigen::Vector2i  the position of the corresponding cell in the grid
	 */
	Eigen::Vector2i grid_position(const Eigen::Vector2d p) const;

	/**
	 * @brief Affectation operator
	 *
	 * @param g         the grid to affect
	 * @return Grid2d&  a reference to this grid
	 */
	Grid2d& operator=(const Grid2d& g);
	/**
	 * @brief Affectation operator
	 *
	 * @param g         the grid to affect
	 * @return Grid2d&  a reference to this grid
	 */
	Grid2d& operator=(Grid2d&& g);

protected:
	int _grid_width;            /**< the number of cells on the width of the grid*/
	int _grid_height;           /**< the number of cells on the height of the grid*/
	Eigen::Vector2d _cell_size; /**< the size of a cell on the plane*/
};
