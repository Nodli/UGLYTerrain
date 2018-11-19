#include <ScalarField.hpp>
#include <algorithm>

double ScalarField::get_value(const double x, const double y) const
{
	Eigen::Vector2i ij = grid_position(x, y);
	Eigen::Vector2d pij = position(ij(0), ij(1));
	Eigen::Vector2d diff(x, y);
	diff -= pij;
	double u = diff(0) / _cell_size(0);
	double v = diff(1) / _cell_size(1);
	return (1 - u) * (1 - v) * get_value(ij(0), ij(1)) + (1 - u) * v * get_value(ij(0), ij(1) + 1) + u * (1 - v) * get_value(ij(0) + 1, ij(1)) + u * v * get_value(ij(0) + 1, ij(1) + 1);
}


ScalarField ScalarField::get_slope_map() const
{
	ScalarField sf(static_cast<Grid2d>(*this));

	for(int j = 0; j < _grid_height; ++j)
	{
		for(int i = 0; i < _grid_width; ++i)
		{
			sf.at(i, j) = slope(i, j);
		}
	}

	return std::move(sf);
}

double ScalarField::slope(const int i, const int j) const
{
	Eigen::Vector2d grad = gradient(i, j);
	return sqrt(grad.dot(grad));
}

Eigen::Vector2d ScalarField::gradient(const int i, const int j) const
{
	Eigen::Vector2d grad;
	double delta_x = width() / _grid_width;
	double delta_y = height() / _grid_height;
	double scale = 1;

	if(i <= 0)
	{
		grad(0) = (get_value(i + 1, j) - get_value(i, j)) / delta_x;
	}
	else if(i >= _grid_width - 1)
	{
		grad(0) = (get_value(i, j) - get_value(i - 1, j)) / delta_x;
	}
	else
	{
		grad(0) = (get_value(i + 1, j) - get_value(i - 1, j)) / (2.0 * delta_x);
	}

	if(j <= 0)
	{
		grad(1) = (get_value(i, j + 1) - get_value(i, j)) / delta_y;
	}
	else if(j >= _grid_height - 1)
	{
		grad(1) = (get_value(i, j) - get_value(i, j - 1)) / delta_y;
	}
	else
	{
		grad(1) = (get_value(i, j + 1) - get_value(i, j - 1)) / (2.0 * delta_y);
	}

	return scale * grad;
}

Eigen::Vector3d ScalarField::normal(const int i, const int j) const
{
	Eigen::Vector3d result;
	Eigen::Vector2d grad = gradient(i, j);
	result[0] = grad[0];
	result[1] = grad[1];
	result[2] = -1;
	result.normalize();
	return result;
}

void ScalarField::set_value(const int i, const int j, double value)
{
	_values.at(index(i, j)) = value;
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


void ScalarField::exportAsObj(const std::string filename, const std::string name) const
{
	// Open the output file
	std::ofstream output(filename, std::ofstream::out);

	// Give the name to the object if specified
	if(name != "")
	{
		output << "o " << name << std::endl;
	}

	// Set the information for each points
	for(int j = 0; j < _grid_height; ++j)
	{
		// a => the lower left corner of the field in world coordinate
		// Calculate the y coordinate
		double vy = _a[1] + j * _cell_size[1];

		for(int i = 0; i < _grid_width; ++i)
		{
			// Compute the normal of the point
			Eigen::Vector3d norm = normal(i, j);
			// Set the vertex information
			output << "v " << _a[0] + i * _cell_size[0] << " " << get_value(i, j) << " " << vy << std::endl;
			// Set the texture information
			output << "vt " << (double)i / (_grid_width - 1) << " " << (double)j / (_grid_height - 1) << std::endl;
			// Set the normal information
			output << "vn " << norm[0] << " " << norm[2] << " " << norm[1] << std::endl;
			output << std::endl;
		}
	}

	// Set the information for each face
	for(int j = 0; j < _grid_height - 1; ++j)
	{
		for(int i = 1; i < _grid_width; ++i)
		{
			int id0 = j * _grid_width + i;
			int id1 = j * _grid_width + i + 1;
			int id2 = (j + 1) * _grid_width + i;
			output << "f " << id0 << "/" << id0 << "/" << id0;
			output << " " << id1 << "/" << id1 << "/" << id1;
			output << " " << id2 << "/" << id2 << "/" << id2 << std::endl;
			id0 = j * _grid_width + i + 1;
			id1 = (j + 1) * _grid_width + i + 1;
			id2 = (j + 1) * _grid_width + i;
			output << "f " << id0 << "/" << id0 << "/" << id0;
			output << " " << id1 << "/" << id1 << "/" << id1;
			output << " " << id2 << "/" << id2 << "/" << id2 << std::endl;
		}
	}

	output.close();
}


void ScalarField::exportAsPgm(const std::string filename, bool minMax, double rangeMin, double rangeMax) const
{
	int maxVal = 255;
	std::ofstream output(filename, std::ofstream::out);
	output << "P2" << std::endl;
	output << _grid_width << " " << _grid_height << std::endl;
	output << maxVal << std::endl;

	if(minMax)
	{
		rangeMax = *max_element(_values.begin(), _values.end());
		rangeMin = *min_element(_values.begin(), _values.end());
	}

	float range = rangeMax - rangeMin;

	for(int j = _grid_height - 1; j >= 0; --j)
	{
		for(int i = 0; i < _grid_width; ++i)
		{
			output << static_cast<int>(((std::max((std::min(get_value(i, j), rangeMax) - rangeMin), 0.) / range)*maxVal)) << " ";
		}

		output << std::endl;
	}
}