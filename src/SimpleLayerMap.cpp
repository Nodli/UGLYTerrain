#include <SimpleLayerMap.hpp>
#include <algorithm>

SimpleLayerMap SimpleLayerMap::generate_slope_map(const DoubleField& field)
{
	SimpleLayerMap sf(static_cast<Grid2d>(field));

	for(int j = 0; j < field.grid_height(); ++j)
	{
		for(int i = 0; i < field.grid_width(); ++i)
		{
			sf.at(i, j) = field.slope(i, j);
		}
	}

	return sf;
}

void SimpleLayerMap::set_value(const int i, const int j, double value)
{
	at(i, j) = value;
}

void SimpleLayerMap::set_all(const double value)
{
	std::fill(_values.begin(), _values.end(), value);
}

SimpleLayerMap& SimpleLayerMap::normalize()
{
	double range = get_range();
	double min = get_min();

	for(int i = 0; i < _values.size(); i++)
	{
		_values[i] = (_values[i] - min) / range;
	}

	return *this;
}

void SimpleLayerMap::copy_values(const SimpleLayerMap& sf)
{
	if(_grid_height == sf._grid_height && _grid_width == sf._grid_width)
	{
		_values = sf._values;
	}
	else
	{
		throw std::invalid_argument("Wrong SimpleLayerMap size");
	}
}

void SimpleLayerMap::copy_values(SimpleLayerMap&& sf)
{
	if(_grid_height == sf._grid_height && _grid_width == sf._grid_width)
	{
		_values = std::move(sf._values);
	}
	else
	{
		throw std::invalid_argument("Wrong SimpleLayerMap size");
	}
}

SimpleLayerMap& SimpleLayerMap::operator=(const SimpleLayerMap& sf)
{
	if(this != &sf)
	{
		Grid2d::operator=(sf);
		this->_values = sf._values;
	}

	return *this;
}

SimpleLayerMap& SimpleLayerMap::operator=(SimpleLayerMap&& sf)
{
	if(this != &sf)
	{
		Grid2d::operator=(sf);
		this->_values = std::move(sf._values);
	}

	return *this;
}

SimpleLayerMap& SimpleLayerMap::operator+=(const SimpleLayerMap& sf)
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

SimpleLayerMap operator+(SimpleLayerMap lsf, const SimpleLayerMap& rsf)
{
	lsf += rsf;
	return lsf;
}

SimpleLayerMap& SimpleLayerMap::operator+=(const double& d)
{
	for(int i = 0; i < this->_values.size(); ++i)
	{
		this->_values[i] += d;
	}

	return *this;
}

SimpleLayerMap operator+(SimpleLayerMap lsf, const double& rd)
{
	lsf += rd;
	return lsf;
}

SimpleLayerMap& SimpleLayerMap::operator-=(const SimpleLayerMap& sf)
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

SimpleLayerMap operator-(SimpleLayerMap lsf, const SimpleLayerMap& rsf)
{
	lsf -= rsf;
	return lsf;
}

SimpleLayerMap& SimpleLayerMap::operator-=(const double& d)
{
	for(int i = 0; i < this->_values.size(); ++i)
	{
		this->_values[i] -= d;
	}

	return *this;
}

SimpleLayerMap operator-(SimpleLayerMap lsf, const double& rd)
{
	lsf -= rd;
	return lsf;
}

SimpleLayerMap operator-(const double& ld, SimpleLayerMap rsf)
{
	rsf -= ld;
	return rsf;
}

SimpleLayerMap& SimpleLayerMap::operator*=(const SimpleLayerMap& sf)
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

SimpleLayerMap operator*(SimpleLayerMap lsf, const SimpleLayerMap& rsf)
{
	lsf *= rsf;
	return lsf;
}

SimpleLayerMap& SimpleLayerMap::operator*=(const double& d)
{
	for(int i = 0; i < this->_values.size(); ++i)
	{
		this->_values[i] *= d;
	}

	return *this;
}

SimpleLayerMap operator*(SimpleLayerMap lsf, const double& rd)
{
	lsf *= rd;
	return lsf;
}
