#include <MultiLayerMap.hpp>
ScalarField& MultiLayerMap::new_field()
{
    add_field(ScalarField(_grid_width, _grid_height, _a, _b));
    return _fields.back();
}

void MultiLayerMap::reshape(double ax, double ay, double bx, double by)
{
	Grid2d::reshape(ax, ay, bx, by);

	for(int i = 0; i < _fields.size(); ++i)
	{
		_fields.at(i).reshape(ax, ay, bx, by);
	}
}

void MultiLayerMap::reshape(const Eigen::Vector2d a, const Eigen::Vector2d b)
{
	Grid2d::reshape(a, b);

	for(int i = 0; i < _fields.size(); ++i)
	{
		_fields.at(i).reshape(a, b);
	}
}

void MultiLayerMap::erode(double k) {
    for (int j = 0; j < _grid_height; ++j)
    {
        for (int i = 0; i < _grid_width; ++i)
        {
            _fields.front().at(i, j) -= _fields.front().slope_erosion(i, j, k);
        }
    }
}

void MultiLayerMap::erode_and_create(double k) {
    if(_fields.size() > 1) 
    {
        for (int j = 0; j < _grid_height; ++j)
        {
            for (int i = 0; i < _grid_width; ++i)
            {
                double eroded_quantity = _fields.front().slope_erosion(i, j, k);
                _fields.front().at(i, j) -= eroded_quantity;
                _fields[1].at(i, j) += eroded_quantity;
            }
        }
    }
}

MultiLayerMap& MultiLayerMap::operator=(const MultiLayerMap &mlm)
{
	Grid2d::operator=(mlm);
	_fields = mlm._fields;
	return *this;
}

MultiLayerMap& MultiLayerMap::operator=(MultiLayerMap &&mlm)
{
	if(this != &mlm)
	{
		Grid2d::operator=(std::move(mlm));
		_fields = std::move(mlm._fields);
	}

	return *this;
}

ScalarField MultiLayerMap::generate_field() const
{
	ScalarField result(*this);

	for(int i = 0; i < _grid_width; ++i)
	{
		for(int j = 0; j < _grid_height; ++j)
		{
			result.set_value(i, j, 0);

			for(int f = 0; f < _fields.size(); ++f)
			{
				result.at(i, j) += _fields[f].get_value(i, j);
			}
		}
	}

	return std::move(result);
}
