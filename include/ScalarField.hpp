#pragma once

#include <Grid2d.hpp>

#include <vector>
#include <fstream>

/**
 * @brief Defines a field of values spread across a grid on the plane
 *
 */
class ScalarField : public Grid2d
{
public:
	ScalarField() = delete;
	/**
	 * @brief Construct a new Scalar Field object from an other Scalar Field
	 *
	 * @param hf        the Scalar field to copy
	 */
	ScalarField(const ScalarField& hf)
		: Grid2d(hf), _values(hf._values) {}
	/**
	 * @brief Construct a new Scalar Field object from an other Scalar Field
	 *
	 * @param hf        the Scalar Field to copy
	 */
	ScalarField(ScalarField&& hf)
		: Grid2d(std::move(hf)), _values(std::move(hf._values)) {}
	/**
	 * @brief Construct a new empty Scalar Field object from a grid
	 *
	 * @param g         the initial grid of the Scalar Field
	 */
	ScalarField(const Grid2d &g)
		: Grid2d(g)
	{
		_values.resize(g.cell_number());
	}
	/**
	 * @brief Construct a new Scalar Field object from a box
	 *
	 * @param b         the box in wich to create the grid
	 * @param width     the number of cells along the width of the grid
	 * @param height    the number of cells along the height of the grid
	 */
	ScalarField(const Box2d &b, const int width, const int height)
		: Grid2d(b, width, height)
	{
		_values.resize(width * height);
	}
	/**
	 * @brief Construct a new Scalar Field object
	 *
	 * @param width     the number of cells along the width of the grid
	 * @param height    the number of cells along the height of the grid
	 * @param a         the first point of the grid
	 * @param b         the second point of the grid
	 */
	ScalarField(const int width, const int height, const Eigen::Vector2d a = {0, 0}, const Eigen::Vector2d b = {1, 1})
		: Grid2d(width, height, a, b)
	{
		_values.resize(width * height);
	}

	/**
	 * @brief Get the interpolated value of the field at a given position on the plane
	 *
	 * @param x, y      the position of a point on the plane
	 * @return double   the interpolated value of the field at that position
	 */
	double value(const double x, const double y) const;
	/**
	 * @brief Get the value of the field at a given cell
	 *
	 * @param i, j      the position of the cell on the grid
	 * @return double   the exact value of the field in that cell
	 */
	double value(const int i, const int j) const
	{
		return _values.at(index(i, j));
	}
	/**
	 * @brief Get the value of the field at a given cell
	 *
	 * @param p         the position of the cell on the grid
	 * @return double   the exact value of the field in that cell
	 */
	double value(const Eigen::Vector2i p) const
	{
		return value(p(0), p(1));
	}

	/**
	 * @brief Calculate the slope of the field at a given cell.
	 * The slope is calculated to be the norm of the gradient
	 *
	 * @param p         the position of the cell on the grid
	 * @return double   the value of the slope at that cell
	 */
	double slope(const Eigen::Vector2i p) const;
	/**
	 * @brief Calculate the slope of the field at a given cell.
	 * The slope is calculated to be the norm of the gradient
	 *
	 * @param i, j      the position of the cell on the grid
	 * @return double   the value of the slope at that cell
	 */
	double slope(const int i, const int j) const;

	/**
	 * @brief Generate a Scalar Field containing the slope information
	 *
	 * @return ScalarField  the field of slopes.
	 */
	ScalarField get_slope_map() const;

	/**
	 * @brief Calculate the gradient of the field at a given cell
	 *
	 * @param i, j      the position of the cell on the grid
	 * @return Eigen::Vector2d  the gradient at that cell
	 */
	Eigen::Vector2d gradient(const int i, const int j) const;
	/**
	 * @brief Calculate the normal to the gradient at a given cell
	 *
	 * @param i, j      the position of the cell on the grid
	 * @return Eigen::Vector3d  the normal at that cell
	 */
	Eigen::Vector3d normal(const int i, const int j) const;

	/**
	 * @brief Gets acess to a cell of the field
	 *
	 * @param i, j      the position of the cell on the grid
	 * @return double&  a reference to the value of that cell
	 */
	double& at(const int i, const int j)
	{
		return _values.at(index(i, j));
	}

	/**
	 * @brief Get all the information of a neigborhood
	 *
	 * @param pos       the position of the cell on the grid
	 * @param v         the value of the neighbors
	 * @param p         the positions of the neighbors
	 * @param s         the slopes of the neighbors
	 * @return int      the number of neigbors
	 */
	int neighbors_info(const Eigen::Vector2i pos, double v[8], Eigen::Vector2i p[8], double s[8]) const
	{
		return neighbors_info(pos(0), pos(1), v, p, s);
	}
	/**
	 * @brief Get all the information of a neigborhood
	 *
	 * @param i, j      the position of the cell on the grid
	 * @param v         the value of the neighbors
	 * @param p         the positions of the neighbors
	 * @param s         the slopes of the neighbors
	 * @return int      the number of neigbors
	 */
	int neighbors_info(const int i, const int j, double v[8], Eigen::Vector2i p[8], double s[8]) const;

	/**
	 * @brief Set the value of a cell of the field
	 *
	 * @param i, j      the position of the cell on the grid
	 * @param value     the value to which set the field
	 */
	void set_value(const int i, const int j, double value);

	/**
	 * @brief Copies the values from an other field
	 *
	 * @param sf        the scalar field to copy the values from
	 */
	void copy_values(const ScalarField& sf);
	/**
	 * @brief steels the values from an other field
	 *
	 * @param sf        the scalar field to steal the values from
	 */
	void copy_values(ScalarField&& sf);

	/**
	 * @brief Affectation operator
	 *
	 * @param sf            the Scalar Field to affect
	 * @return ScalarField& a reference to this Scalar Field
	 */
	ScalarField& operator=(const ScalarField& sf);
	/**
	 * @brief Affectation operatof
	 *
	 * @param sf            the Scalar field to affect
	 * @return ScalarField& a reference to this Scalar Field
	 */
	ScalarField& operator=(ScalarField&& sf);

	/**
	 * @brief Export the Scalar Field as a obj
	 *
	 * @param filename      the name of the file
	 * @param name          the name of the object in the file
	 */
	void export_as_obj(const std::string filename, std::string name = "") const;

	/**
	 * @brief Export the Scalar Field as a pgm
	 *
	 * @param filename      the name of the file
	 * @param minMan        tells of the values should be ranged automatically
	 * @param rangeMin      give a minimal value
	 * @param rangeMax      give a maximal value
	 */
	void export_as_pgm(const std::string filename, bool minMan = true, double rangeMin = 0, double rangeMax = 1) const;

	/**
	 * @brief export the ScalarField as a list of pairs <value/position>
	 *
	 * @return std::vector<std::pair<double, Eigen::Vector2i>>  a list of pair <value/cell_position> representing the Scalar Field
	 */
	std::vector<std::pair<double, Eigen::Vector2i>> export_to_list() const;

protected:
	/**
	 * @brief Gets the index on the array of values for a given position
	 *
	 * @param i, j      the position on the grid
	 * @throw           invalid_argument if the position required is not on the grid
	 * @return int      the index of that positition in the array of values
	 */
	int index(const int i, const int j) const
	{
		int index = _grid_width * j + i;

		if(index >= cell_number())
		{
			throw std::invalid_argument("wrong access to a value in the ScalarField");
		}

		return index;
	}

	std::vector<double> _values;    /**< array containing all the values of the field*/
};
