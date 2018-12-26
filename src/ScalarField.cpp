#include <ScalarField.hpp>
#include <algorithm>

ScalarField ScalarField::generate_slope_map(const DoubleField& field)
{
	ScalarField sf(static_cast<Grid2d>(field));

	for(int j = 0; j < field.grid_height(); ++j)
	{
		for(int i = 0; i < field.grid_width(); ++i)
		{
			sf.at(i, j) = field.slope(i, j);
		}
	}

	return sf;	
}

void ScalarField::set_value(const int i, const int j, double value)
{
	at(i, j) = value;
}

void ScalarField::set_all(const double value)
{
	std::fill(_values.begin(), _values.end(), value);
}

void ScalarField::normalize()
{
	double range = get_range();
	double min = get_min();

	for(int i = 0; i < _values.size(); i++)
		_values[i] = (_values[i] - min) / range;
}

void ScalarField::copy_values(const ScalarField& sf)
{
	if(_grid_height == sf._grid_height && _grid_width == sf._grid_width)
	{
		_values = sf._values;
	}
	else
	{
		throw std::invalid_argument("Wrong ScalarField size");
	}
}

void ScalarField::copy_values(ScalarField&& sf)
{
	if(_grid_height == sf._grid_height && _grid_width == sf._grid_width)
	{
		_values = std::move(sf._values);
	}
	else
	{
		throw std::invalid_argument("Wrong ScalarField size");
	}
}

ScalarField& ScalarField::operator=(const ScalarField& sf)
{
	if(this != &sf)
	{
		Grid2d::operator=(sf);
		this->_values = sf._values;
	}

	return *this;
}

ScalarField& ScalarField::operator=(ScalarField&& sf)
{
	if(this != &sf)
	{
		Grid2d::operator=(sf);
		this->_values = std::move(sf._values);
	}

	return *this;
}

ScalarField& ScalarField::operator+=(const ScalarField& sf)
{
	if(this->_values.size() == sf._values.size())
	{
		for(int i = 0; i < this->_values.size(); ++i)
		{
			this->_values[i] += sf._values[i];
		}
	}

	return *this;
}

ScalarField operator+(ScalarField lsf, const ScalarField& rsf)
{
	lsf += rsf;
	return lsf;
}

ScalarField& ScalarField::operator-=(const ScalarField& sf)
{
	if(this->_values.size() == sf._values.size())
	{
		for(int i = 0; i < this->_values.size(); ++i)
		{
			this->_values[i] -= sf._values[i];
		}
	}

	return *this;
}

ScalarField operator-(ScalarField lsf, const ScalarField& rsf)
{
	lsf -= rsf;
	return lsf;
}

ScalarField& ScalarField::operator*=(const ScalarField& sf)
{
	if(this->_values.size() == sf._values.size())
	{
		for(int i = 0; i < this->_values.size(); ++i)
		{
			this->_values[i] *= sf._values[i];
		}
	}

	return *this;
}

ScalarField operator*(ScalarField lsf, const ScalarField& rsf)
{
	lsf *= rsf;
	return lsf;
}

ScalarField& ScalarField::operator*=(const double& d)
{
	for(int i = 0; i < this->_values.size(); ++i)
	{
		this->_values[i] *= d;
	}

	return *this;
}

ScalarField operator*(ScalarField lsf, const double& rd)
{
	lsf *= rd;
	return lsf;
}
