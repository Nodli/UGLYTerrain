#include <DoubleField.hpp>
#include <algorithm>
#include <iostream>


DoubleField::read_only_iterator DoubleField::begin() const
{
	return DoubleField::read_only_iterator(this, 0);
}

DoubleField::read_only_iterator DoubleField::end() const
{
	return DoubleField::read_only_iterator(this, cell_number());
}

double DoubleField::value_inter(const double x, const double y) const
{
	Eigen::Vector2i ij = grid_position(x, y);
	Eigen::Vector2d pij = position(ij(0), ij(1));
	Eigen::Vector2d diff(x, y);
	diff -= pij;
	double u = diff(0) / _cell_size(0);
	double v = diff(1) / _cell_size(1);
	return (1 - u) * (1 - v) * value(ij(0), ij(1)) + (1 - u) * v * value(ij(0), ij(1) + 1) + u * (1 - v) * value(ij(0) + 1, ij(1)) + u * v * value(ij(0) + 1, ij(1) + 1);
}

double DoubleField::get_min() const
{
	return *std::min_element(begin(), end());
}

double DoubleField::get_max() const
{
	return *std::max_element(begin(), end());
}

double DoubleField::get_range() const
{
	double valMin = get_min();
	double valMax = get_max();
	return valMax - valMin;
}

double DoubleField::get_sum() const
{
	double sum = 0;

	for(int j = 0; j < grid_height(); ++j)
	{
		for(int i = 0; i < grid_width(); i++)
		{
			sum += value(i, j);
		}
	}

	return sum;
}

double DoubleField::slope(const Eigen::Vector2i p) const
{
	return slope(p(0), p(1));
}
double DoubleField::slope(const int i, const int j) const
{
	Eigen::Vector2d grad = gradient(i, j);
	return sqrt(grad.dot(grad));
}

Eigen::Vector2d DoubleField::gradient(const int i, const int j) const
{
	Eigen::Vector2d grad;
	double delta_x = width() / _grid_width;
	double delta_y = height() / _grid_height;
	double scale = 1;

	if(i <= 0)
	{
		grad(0) = (value(i + 1, j) - value(i, j)) / delta_x;
	}
	else if(i >= _grid_width - 1)
	{
		grad(0) = (value(i, j) - value(i - 1, j)) / delta_x;
	}
	else
	{
		grad(0) = (value(i + 1, j) - value(i - 1, j)) / (2.0 * delta_x);
	}

	if(j <= 0)
	{
		grad(1) = (value(i, j + 1) - value(i, j)) / delta_y;
	}
	else if(j >= _grid_height - 1)
	{
		grad(1) = (value(i, j) - value(i, j - 1)) / delta_y;
	}
	else
	{
		grad(1) = (value(i, j + 1) - value(i, j - 1)) / (2.0 * delta_y);
	}

	return scale * grad;
}

Eigen::Vector3d DoubleField::normal(const int i, const int j) const
{
	Eigen::Vector3d result;
	Eigen::Vector2d grad = gradient(i, j);
	result[0] = -grad[0];
	result[1] = -grad[1];
	result[2] = 1;
	result.normalize();
	return result;
}

std::vector<std::pair<double, Eigen::Vector2i>> DoubleField::sort_by_height() const
{
	std::vector<std::pair<double, Eigen::Vector2i>> sorted_indices = export_to_list();
	std::sort(sorted_indices.begin(), sorted_indices.end(), [](const std::pair<double, Eigen::Vector2i>& a, const std::pair<double, Eigen::Vector2i>& b)
	{
		return a.first > b.first;
	});
	return sorted_indices;
}

int DoubleField::neighbors_info(const int i, const int j, double* v, Eigen::Vector2i* p, double* s) const
{
	Eigen::Vector2i* positions;
	if(p == nullptr){
		positions = new Eigen::Vector2i[8]; // temp allocation because values need to be used for v[k]
	}else{
		positions = p;
	}

	int nb = neighbors(i, j, positions);
	double ij_value = value(i, j);

	for(int k = 0; k < nb; ++k)
	{
		if(v != nullptr){
			v[k] = value(positions[k]);
		}

		if(s != nullptr){
			s[k] = (v[k] - ij_value) / def_nei_dist[k];
		}
	}

	if(p == nullptr){
		delete positions;
	}

	return nb;
}

int DoubleField::neighbors_info_filter(const int i, const int j, double* v, Eigen::Vector2i* p, double* s, const double s_filter, const bool sup) const
{
	int nb = neighbors(i, j, p);
	int threshold_nb = 0;
	double ij_value = value(i, j);

	for(int ineigh = 0; ineigh < nb; ++ineigh)
	{
		// values are computed in place but will be overridden / not considered if threshold_nb is not incremented
		p[threshold_nb] = p[ineigh];
		v[threshold_nb] = value(p[threshold_nb]);
		s[threshold_nb] = (v[threshold_nb] - ij_value) / def_nei_dist[ineigh];

		if(sup)
		{
			if(s[threshold_nb] > s_filter)
			{
				++threshold_nb;
			}
		}
		else
		{
			if(s[threshold_nb] < s_filter)
			{
				++threshold_nb;
			}
		}
	}

	return threshold_nb;
}


DoubleField& DoubleField::operator=(const DoubleField& sf)
{
	if(this != &sf)
	{
		Grid2d::operator=(sf);
	}

	return *this;
}

DoubleField& DoubleField::operator=(DoubleField&& sf)
{
	if(this != &sf)
	{
		Grid2d::operator=(sf);
	}

	return *this;
}

void DoubleField::export_as_obj(const std::string filename, const std::string name) const
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
			output << "v " << _a[0] + i * _cell_size[0] << " " << value(i, j) << " " << vy << std::endl;
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


void DoubleField::export_as_pgm(const std::string filename, bool minMax, double rangeMin, double rangeMax) const
{
	int maxVal = 255;
	std::ofstream output(filename, std::ofstream::out);
	output << "P2" << std::endl;
	output << _grid_width << " " << _grid_height << std::endl;
	output << maxVal << std::endl;

	if(minMax)
	{
		rangeMax = get_max();
		rangeMin = get_min();
	}

	float range = rangeMax - rangeMin;

	for(int j = _grid_height - 1; j >= 0; --j)
	{
		for(int i = 0; i < _grid_width; ++i)
		{
			output << static_cast<int>(((std::max((std::min(value(i, j), rangeMax) - rangeMin), 0.) / range)*maxVal)) << " ";
		}

		output << std::endl;
	}
}


std::vector<std::pair<double, Eigen::Vector2i>> DoubleField::export_to_list() const
{
	std::vector<std::pair<double, Eigen::Vector2i>> field;

	for(int j = 0; j < _grid_height; ++j)
	{
		for(int i = 0 ; i < _grid_width; ++i)
		{
			field.push_back(std::make_pair(value(i, j), Eigen::Vector2i(i, j)));
		}
	}

	return field;
}

double DoubleField::read_only_iterator::operator*() const
{
	return _parent->value(_parent->posi_from_index(_position));
}

DoubleField::read_only_iterator& DoubleField::read_only_iterator::operator++()
{
	_position++;
	return *this;
}

DoubleField::read_only_iterator DoubleField::read_only_iterator::operator++(int i)
{
	read_only_iterator roi = *this;
	_position++;
	return roi;
}

bool DoubleField::read_only_iterator::operator==(const read_only_iterator& roi) const
{
	return _parent == roi._parent && _position == roi._position;
}

bool DoubleField::read_only_iterator::operator!=(const read_only_iterator& roi) const
{
	return !(*this == roi);
}

DoubleField::read_only_iterator& DoubleField::read_only_iterator::operator=(const read_only_iterator& roi)
{
	_parent = roi._parent;
	_position = roi._position;
	return *this;
}
