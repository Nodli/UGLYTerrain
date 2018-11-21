#pragma once

#include <vector>

/**
 * @brief Defines a field of values spread across a grid on the plane
 *
 */
class BooleanField
{
public:
	BooleanField() = delete;
	/**
	 * @brief Construct a new Scalar Field object from an other Scalar Field
	 *
	 * @param bf        the Scalar field to copy
	 */
	BooleanField(const BooleanField& bf)
		: _width(bf._width), _height(bf._height), _values(bf._values) {}
	/**
	 * @brief Construct a new Scalar Field object from an other Scalar Field
	 *
	 * @param hf        the Scalar Field to copy
	 */
	BooleanField(BooleanField&& bf)
		: _width(bf._width), _height(bf._height), _values(std::move(bf._values)) {}
	/**
	 * @brief Construct a new Scalar Field object
	 *
	 * @param width     the number of cells along the width of the grid
	 * @param height    the number of cells along the height of the grid
	 * @param a         the first point of the grid
	 * @param b         the second point of the grid
	 */
	BooleanField(const int width, const int height, const bool default_value = false)
		: _width(width), _height(height)
	{
		_values.resize(width * height, default_value);
	}

	/**
	 * @brief Get the value (copy) of the field at a given cell
	 *
	 * @param i, j      the position of the cell on the grid
	 * @return double   the exact value of the field in that cell
	 */
	bool value(const int i, const int j) const
	{
		return static_cast<bool>(_values.at(index(i, j)));
	}
	/**
	 * @brief Get the value (copy) of the field at a given cell
	 *
	 * @param p         the position of the cell on the grid
	 * @return double   the exact value of the field in that cell
	 */
	bool value(const Eigen::Vector2i p) const
	{
		return value(p(0), p(1));
	}

	/**
	 * @brief Gets acess to a cell of the field
	 *
	 * @param p      the position of the cell on the grid
	 * @return double&  a reference to the value of that cell
	 */
	char& at(const Eigen::Vector2i p)
	{
		return at(p(0), p(1));
	}

	/**
	 * @brief Gets acess to a cell of the field
	 *
	 * @param i, j      the position of the cell on the grid
	 * @return double&  a reference to the value of that cell
	 */
	char& at(const int i, const int j)
	{
		return _values.at(index(i, j));
	}

	/**
	 * @brief Sets all the cell to value
	 *
	 */
	void set_all(bool value = false)
	{
		std::fill(_values.begin(), _values.end(), value);
	}

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
		int index = _width * j + i;

		if(index >= _width * _height)
		{
			throw std::invalid_argument("wrong access to a value in the BooleanField");
		}

		return index;
	}

	int _width;
	int _height;
	std::vector<char> _values;    /**< array containing all the values of the field*/
};
