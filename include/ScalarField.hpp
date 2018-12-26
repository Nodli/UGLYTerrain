#pragma once

#include <DoubleField.hpp>

#include <vector>
#include <fstream>

/**
 * @brief Defines a field of values spread across a grid on the plane
 *
 */
class ScalarField : public DoubleField
{
public: 
	static ScalarField generate_slope_map(const DoubleField& field);
public:
	ScalarField() = delete;
	/**
	 * @brief Construct a new Scalar Field object from an other Scalar Field
	 *
	 * @param hf        the Scalar field to copy
	 */
	ScalarField(const ScalarField& hf)
		: DoubleField(hf), _values(hf._values) {}
	/**
	 * @brief Construct a new Scalar Field object from an other Scalar Field
	 *
	 * @param hf        the Scalar Field to copy
	 */
	ScalarField(ScalarField&& hf)
		: DoubleField(std::move(hf)), _values(std::move(hf._values)) {}
	/**
	 * @brief Construct a new empty Scalar Field object from a grid
	 *
	 * @param g         the initial grid of the Scalar Field
	 */
	ScalarField(const Grid2d &g)
		: DoubleField(g)
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
		: DoubleField(b, width, height)
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
		: DoubleField(width, height, a, b)
	{
		_values.resize(width * height);
	}

	/**
	 * @brief Get the value of the field at a given cell
	 *
	 * @param i, j      the position of the cell on the grid
	 * @return double   the exact value of the field in that cell
	 */
	virtual double value(const int i, const int j) const
	{
		return _values.at(index(i, j));
	}

	/**
	 * @brief Set the value of a cell of the field
	 *
	 * @param i, j      the position of the cell on the grid
	 * @param value     the value to which set the field
	 */
	void set_value(const int i, const int j, double value);
	
	/**
	 * @brief Gets acess to a cell of the field
	 *
	 * @param i, j      the position of the cell on the grid
	 * @return double&  a reference to the value of that cell
	 */
	double& at(const Eigen::Vector2i p)
	{
		return at(p(0), p(1));
	}
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
	 * @brief Set the value all cells in the field
	 *
	 * @param value     the value to set the field to
	 */
	void set_all(const double value);

	/**
	 * @brief Normalize the values of the field
	 */
	void normalize();

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
	 * @brief Affectation operator
	 *
	 * @param sf            the Scalar field to affect
	 * @return ScalarField& a reference to this Scalar Field
	 */
	ScalarField& operator=(ScalarField&& sf);
	/**
	 * @brief Addition assignment operator
	 *
	 * @param sf            the Scalar field to add
	 * @return ScalarField& a reference to this Scalar Field
	 */
	ScalarField& operator+=(const ScalarField& sf);
	/**
	 * @brief Addition operator
	 *
	 * @param lsf            the Scalar field to add on the left
	 * @param rsf            the Scalar field to add on the right
	 * @return ScalarField, result of the addition, by value
	 */
	friend ScalarField operator+(ScalarField lsf, const ScalarField& rsf);
	/**
	 * @brief Substraction assignment operator
	 *
	 * @param sf            the Scalar field to substract
	 * @return ScalarField& a reference to this Scalar Field
	 */
	ScalarField& operator-=(const ScalarField& sf);
	/**
	 * @brief Substraction operator
	 *
	 * @param lsf            the Scalar field to substract on the left
	 * @param rsf            the Scalar field to substract on the right
	 * @return ScalarField, result of the substraction, by value
	 */
	friend ScalarField operator-(ScalarField lsf, const ScalarField& rsf);
	/**
	 * @brief Multiplication assignment operator
	 *
	 * @param sf            the Scalar field to multiply
	 * @return ScalarField& a reference to this Scalar Field
	 */
	ScalarField& operator*=(const ScalarField& sf);
	/**
	 * @brief Multiplication operator
	 *
	 * @param lsf            the Scalar field to multiply on the left
	 * @param rsf            the Scalar field to multiply on the right
	 * @return ScalarField, result of the multiplication, by value
	 */
	friend ScalarField operator*(ScalarField lsf, const ScalarField& rsf);
	/**
	 * @brief Multiplication assignment operator
	 *
	 * @param d            	the double to multiply
	 * @return ScalarField& a reference to this Scalar Field
	 */
	ScalarField& operator*=(const double& d);
	/**
	 * @brief Multiplication operator
	 *
	 * @param lsf            the Scalar field to multiply on the left
	 * @param rd           	 the double to multiply on the right
	 * @return ScalarField, result of the multiplication, by value
	 */
	friend ScalarField operator*(ScalarField lsf, const double& rd);

protected:
	std::vector<double> _values;    /**< array containing all the values of the field*/
};
