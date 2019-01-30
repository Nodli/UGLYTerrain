#pragma once

#include <Grid2d.hpp>
#include <vector>
#include <fstream>
#include <algorithm>

/**
 * @brief Defines a field of values spread across a grid on the plane
 *
 */
class DoubleField : public Grid2d
{
public:
	class read_only_iterator;
	/**
	 * @brief Return a read only iterator to the first element of the field
	 *
	 * @return read_only_iterator   an iterator to the first element of the field
	 */
	read_only_iterator begin() const;

	/**
	 * @brief Return a read only iterator to the last element of the field
	 *
	 * @return read_only_iterator   an iterator to the last element of the field
	 */
	read_only_iterator end() const;

public:
	DoubleField() = delete;
	/**
	 * @brief Construct a new Field object from an other Field
	 *
	 * @param hf        the field to copy
	 */

	DoubleField(const DoubleField& hf)
		: Grid2d(hf) {}
	/**
	 * @brief Construct a new Field object from an other Field
	 *
	 * @param hf        the Field to copy
	 */
	DoubleField(DoubleField&& hf)
		: Grid2d(std::move(hf)) {}

	/**
	 * @brief Construct a new empty Field object from a grid
	 *
	 * @param g         the initial grid of the Field
	 */
	DoubleField(const Grid2d &g)
		: Grid2d(g) {}
	/**
	 * @brief Construct a new Field object from a box
	 *
	 * @param b         the box in wich to create the grid
	 * @param width     the number of cells along the width of the grid
	 * @param height    the number of cells along the height of the grid
	 */
	DoubleField(const Box2d &b, const int width, const int height)
		: Grid2d(b, width, height) {}
	/**
	 * @brief Construct a new Field object
	 *
	 * @param width     the number of cells along the width of the grid
	 * @param height    the number of cells along the height of the grid
	 * @param a         the first point of the grid
	 * @param b         the second point of the grid
	 */
	DoubleField(const int width, const int height, const Eigen::Vector2d a = {0, 0}, const Eigen::Vector2d b = {1, 1})
		: Grid2d(width, height, a, b) {}

	/**
	 * @brief Get the value of the field at a given cell
	 *
	 * @param i, j      the position of the cell on the grid
	 * @return double   the exact value of the field in that cell
	 */
	virtual double value(const int i, const int j) const = 0;

	/**
	 * @brief Get the value of the field at a given cell
	 *    DOES NOT WORK - WE DON'T KNOW WHY
	 *
	 * @param p         the position of the cell on the grid
	 * @return double   the exact value of the field in that cell
	 */
	double value(const Eigen::Vector2i p) const
	{
		return value(p(0), p(1));
	}

	/**
	 * @brief Get the value of the field at a given cell
	 *
	 * @param p         the position of the cell on the grid
	 * @return double   the exact value of the field in that cell or 0 if outside the grid
	 */
	double value_safe(const int i, const int j) const
	{
		if(i < 0 || i >= _grid_width || j < 0 || j >= _grid_height)
		{
			return 0;
		}
		else
		{
			return value(i, j);
		}
	}

	/**
	 * @brief Get the interpolated value of the field at a given position on the plane
	 *
	 * @param x, y      the position of a point on the plane
	 * @return double   the interpolated value of the field at that position
	 */
	double value_inter(const double x, const double y) const;

	/**
	 * @brief Get the min value of the field
	 *
	 * @return double   the min value
	 */
	double get_min() const;

	/**
	 * @brief Get the max value of the field
	 *
	 * @return double   the max value
	 */
	double get_max() const;

	/**
	 * @brief Get range of values in field
	 *
	 * @return double       the range
	 */
	double get_range() const;

	/**
	 * @brief Get sum of values in field
	 *
	 * @return double       the sum of all values
	 */
	double get_sum() const;

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
	 * @brief Get indices of values sorted by height
	 *
	 * @return vector      the indices of values sorted by height
	 */
	std::vector<std::pair<double, Eigen::Vector2i>> sort_by_height() const;

	/**
	 * @brief Get all the information of a neigborhood
	 *
	 * @param pos       the position of the cell on the grid
	 * @param v         the value of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param p         the positions of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param s         the slopes of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @return int      the number of neigbors

	 */
	int neighbors_info(const Eigen::Vector2i pos, double* v, Eigen::Vector2i* p, double* s) const
	{
		return neighbors_info(pos(0), pos(1), v, p, s);
	}

	/**
	 * @brief Get all the information of a neigborhood
	 *
	 * @param i, j      the position of the cell on the grid
	 * @param v         the value of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param p         the positions of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param s         the slopes of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @return int      the number of neigbors
	 */
	int neighbors_info(const int i, const int j, double* v, Eigen::Vector2i* p, double* s) const;

	/**
	 * @brief Get all the information of a neigborhood
	 *
	 * @param pos       the position of the cell on the grid
	 * @param v         the value of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param p         the positions of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param s         the slopes of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @return int      the number of neigbors

	 */
	int neighbors_info_4connex(const Eigen::Vector2i pos, double* v, Eigen::Vector2i* p, double* s) const
	{
		return neighbors_info_4connex(pos(0), pos(1), v, p, s);
	}

	/**
	 * @brief Get all the information of a neigborhood
	 *
	 * @param i, j      the position of the cell on the grid
	 * @param v         the value of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param p         the positions of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param s         the slopes of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @return int      the number of neigbors
	 */
	int neighbors_info_4connex(const int i, const int j, double* v, Eigen::Vector2i* p, double* s) const;

	/**
	 * @brief Get the information of a neigborhood if the slope is superior / inferior to a threshold value
	 *
	 * @param pos       the position of the cell on the grid
	 * @param v         the value of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param p         the positions of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param s         the slopes of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 *          values are signed and the slope vector is oriented from pos towards its neighbors
	 * @param s_filter  the minimal slope value to be considered as a neighbor
	 * @param sup       1 to filter slopes such as s > s_filter and 0 such as s < s_filter using signed values
	 * @return int      the number of neigbors
	 */
	int neighbors_info_filter(const Eigen::Vector2i pos, double* v, Eigen::Vector2i* p, double* s, const double s_filter = 0., const bool sup = false) const
	{
		return neighbors_info_filter(pos(0), pos(1), v, p, s, s_filter, sup);
	}

	/**
	 * @brief Get the information of a neigborhood if the slope is superior / inferior to a threshold value
	 *
	 * @param pos       the position of the cell on the grid
	 * @param v         the value of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param p         the positions of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param s         the slopes of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 *          values are signed and the slope vector is oriented from pos towards its neighbors
	 * @param s_filter  the minimal slope value to be considered as a neighbor
	 * @param sup       1 to filter slopes such as s > s_filter and 0 such as s < s_filter using signed values
	 * @return int      the number of neigbors
	 */
	int neighbors_info_filter_4connex(const Eigen::Vector2i pos, double* v, Eigen::Vector2i* p, double* s, const double s_filter = 0., const bool sup = false) const
	{
		return neighbors_info_filter_4connex(pos(0), pos(1), v, p, s, s_filter, sup);
	}

	/**
	 * @brief Get all the information of a neigborhood
	 *
	 * @param i, j      the position of the cell on the grid
	 * @param v         the value of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param p         the positions of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param s         the slopes of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 *          values are signed and the slope vector is oriented from pos towards its neighbors
	 * @param s_filter  the minimal slope value to be considered as a neighbor
	 * @param sup       1 to filter slopes such as s > s_filter and 0 such as s < s_filter using signed values
	 * @return int      the number of neigbors
	 */
	int neighbors_info_filter(const int i, const int j, double* v, Eigen::Vector2i* p, double* s, const double s_filter = 0., const bool sup = false) const;

	/**
	 * @brief Get all the information of a neigborhood
	 *
	 * @param i, j      the position of the cell on the grid
	 * @param v         the value of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param p         the positions of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 * @param s         the slopes of the neighbors (a pointer to an array of size at least 8 / nullptr)
	 *          values are signed and the slope vector is oriented from pos towards its neighbors
	 * @param s_filter  the minimal slope value to be considered as a neighbor
	 * @param sup       1 to filter slopes such as s > s_filter and 0 such as s < s_filter using signed values
	 * @return int      the number of neigbors
	 */
	int neighbors_info_filter_4connex(const int i, const int j, double* v, Eigen::Vector2i* p, double* s, const double s_filter = 0., const bool sup = false) const;

	/**
	 * @brief Compute the convolution with a given filter for one value
	 * 
	 * @param filter		the filter to apply
	 * @param i, j			the position to apply the filter
	 * @return double		the result of the convolution
	 */
	double convolution(const DoubleField& filter, int i, int j) const;

	/**
	 * @brief Compute the convolution with a given filter for all values
	 * 
	 * @param filter		the filter to apply
	 * @return std::vector<std::pair<double, Eigen::Vector2i>>  a list of pair <value/cell_position> representing the new values
	 */
	std::vector<std::pair<double, Eigen::Vector2i>> full_convolution(const DoubleField& filter) const;

	/**
	 * @brief Affectation operator
	 *
	 * @param sf            the Field to affect
	 * @return DoubleField& a reference to this Field
	 */
	DoubleField& operator=(const DoubleField& sf);

	/**
	 * @brief Affectation operator
	 *
	 * @param sf            the field to affect
	 * @return DoubleField& a reference to this Field
	 */
	DoubleField& operator=(DoubleField&& sf);


	/**
	 * @brief Export the Field as a obj
	 *
	 * @param filename      the name of the file
	 * @param name          the name of the object in the file
	 */
	void export_as_obj(const std::string filename, std::string name = "") const;

	/**
	 * @brief Export the Field as a pgm
	 *
	 * @param filename      the name of the file
	 * @param minMan        tells of the values should be ranged automatically
	 * @param rangeMin      give a minimal value
	 * @param rangeMax      give a maximal value
	 */
	void export_as_pgm(const std::string filename, bool minMan = true, double rangeMin = 0, double rangeMax = 1) const;

	/**
	 * @brief export the Field as a list of pairs <value/position>
	 *
	 * @return std::vector<std::pair<double, Eigen::Vector2i>>  a list of pair <value/cell_position> representing the Field
	 */
	std::vector<std::pair<double, Eigen::Vector2i>> export_to_list() const;
};

/**
 * @brief Class iterator for the double_field.
 * This only allows the reading of the values.
 *
 */
class DoubleField::read_only_iterator
{
public:
	/**
	 * @brief Construct a new read only iterator object
	 *
	 * @param parent        The parent DoubleField
	 * @param position      The position in the doublefield
	 */
	read_only_iterator(const DoubleField* parent, const int position) :
		_parent(parent), _position(position) {}
	/**
	 * @brief Copy a read only iterator object
	 *
	 * @param roi           The iterator to copy
	 */
	read_only_iterator(const read_only_iterator& roi) :
		_parent(roi._parent), _position(roi._position) {}

	/**
	 * @brief Get the value pointed by the iterator
	 *
	 * @return double       The value pointed by the iterator
	 */
	double operator*() const;

	/**
	 * @brief Equality operator between two iterators
	 *
	 * @param roi       the iterator to be compared to
	 * @return true     if the two iterators are equal
	 * @return false    if the two iterators are not equal
	 */
	bool operator==(const read_only_iterator& roi) const;

	/**
	 * @brief Inequality operator
	 *
	 * @param roi       the iterator to be compared to
	 * @return true     if the two iterators are not equal
	 * @return false    if the two iterators are equal
	 */
	bool operator!=(const read_only_iterator& roi) const;

	/**
	 * @brief Incremental operator, points to the next value in the field
	 *
	 * @return read_only_iterator&
	 */
	read_only_iterator& operator++();
	/**
	 * @brief Incremental operator, points to the next value in the field
	 *
	 * @param i
	 * @return read_only_iterator
	 */
	read_only_iterator operator++(int i);
	/**
	 * @brief Affectation operator
	 *
	 * @param roi       The iterator to copy
	 * @return read_only_iterator&
	 */
	read_only_iterator& operator=(const read_only_iterator& roi);

private:
	const DoubleField * _parent;
	int _position;
};
