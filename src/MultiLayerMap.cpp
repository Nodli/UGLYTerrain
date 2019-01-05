#include <MultiLayerMap.hpp>


double MultiLayerMap::value(const int i, const int j) const
{
	double result = 0;

	for(int l = 0; l < get_layer_number(); ++l)
	{
		result += _layers[l].value(i, j);
	}

	return result;
}

SimpleLayerMap& MultiLayerMap::new_layer()
{
	add_field(SimpleLayerMap(_grid_width, _grid_height, _a, _b));
	return _layers.back();
}

void MultiLayerMap::reshape(const double ax, const double ay, const double bx, const double by)
{
	Grid2d::reshape(ax, ay, bx, by);

	for(int i = 0; i < _layers.size(); ++i)
	{
		_layers.at(i).reshape(ax, ay, bx, by);
	}
}

void MultiLayerMap::reshape(const Eigen::Vector2d a, const Eigen::Vector2d b)
{
	Grid2d::reshape(a, b);

	for(int i = 0; i < _layers.size(); ++i)
	{
		_layers.at(i).reshape(a, b);
	}
}

MultiLayerMap& MultiLayerMap::operator=(const MultiLayerMap &mlm)
{
	Grid2d::operator=(mlm);
	_layers = mlm._layers;
	return *this;
}

MultiLayerMap& MultiLayerMap::operator=(MultiLayerMap &&mlm)
{
	if(this != &mlm)
	{
		Grid2d::operator=(std::move(mlm));
		_layers = std::move(mlm._layers);
	}

	return *this;
}

SimpleLayerMap MultiLayerMap::generate_field() const
{
	SimpleLayerMap result(*this);

	for(int i = 0; i < _grid_width; ++i)
	{
		for(int j = 0; j < _grid_height; ++j)
		{
			result.set_value(i, j, 0);

			for(int f = 0; f < _layers.size(); ++f)
			{
				result.at(i, j) += _layers[f].value(i, j);
			}
		}
	}

	return std::move(result);
}
