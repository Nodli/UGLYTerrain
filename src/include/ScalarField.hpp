#pragma once

#include <Grid2d.hpp>

#include <vector>
#include <fstream>

class ScalarField : public Grid2d
{
public:
	ScalarField() = delete;
	ScalarField(const ScalarField& hf)
		: Grid2d(hf), _values(hf._values) {}
	ScalarField(ScalarField&& hf)
		: Grid2d(std::move(hf)), _values(std::move(hf._values)) {}
	ScalarField(const Grid2d &g)
		: Grid2d(g)
	{
		_values.resize(g.cell_number());
	}
	ScalarField(const Box2d &b, const int width, const int height)
		: Grid2d(b, width, height)
	{
		_values.resize(width * height);
	}
	ScalarField(const int width, const int height, const Eigen::Vector2d a = {0, 0}, const Eigen::Vector2d b = {1, 1})
		: Grid2d(width, height, a, b)
	{
		_values.resize(width * height);
	}

	int index(const int i, const int j) const
	{
		return _grid_width * j + i;
	}

	double get_value(const double x, const double y) const;
	double get_value(const int i, const int j) const
	{
		return _values.at(index(i, j));
	}

	double slope(const int i, const int j) const;
	ScalarField get_slope_map() const;
	Eigen::Vector2d gradient(const int i, const int j) const;
	Eigen::Vector3d normal(const int i, const int j) const;

	double& at(const int i, const int j)
	{
		return _values.at(index(i, j));
	}
	void set_value(const int i, const int j, double value);
	void copy_values(const ScalarField& sf);
	void copy_values(ScalarField&& sf);

	ScalarField& operator=(const ScalarField& sf);
	ScalarField& operator=(ScalarField&& sf);

	void exportAsObj(const std::string filename, std::string name = "") const;
	void exportAsPgm(const std::string filename, bool minMan = true, float rangeMin = 0, float rangeMax = 1) const;

	double slope_erosion(int i, int j, double k) { return k * slope(i, j); };

private:
	std::vector<double> _values;
};
