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

double MultiLayerMap::get_sum(const int i, const int j) const
{
	if(i < 0 || i >= _grid_width || j < 0 || j >= _grid_height)
	{
		return 0;
	}

	double res;
	for(int f = 0; f < _layers.size(); ++f)
	{
		res += _layers[f].value(i, j);
	}
	return res;
}

MultiLayerMap normalized(const MultiLayerMap& mlm){
	SimpleLayerMap terrain = mlm.generate_field();
	double whole_max = terrain.get_max();
	double whole_min = mlm.get_field(0).get_min(); // minimum of everything is the minimum of the bedrock layer
	double whole_range = whole_max - whole_min;

	MultiLayerMap output(mlm);
	
	// translating bedrock layer before normalization
	for(int w = 0; w < output.grid_width(); ++w){
		for(int h = 0; h < output.grid_height(); ++h){
			output.get_field(0).at(w, h) -= whole_min;
		}
	}

	// scaling all layers with the global range
	for(int ilayer = 1; ilayer < output.get_layer_number(); ++ilayer){
		for(int w = 0; w < output.grid_width(); ++w){
			for(int h = 0; h < output.grid_height(); ++h){
				output.get_field(ilayer).at(w, h) /= whole_range;
			}
		}
	}

	return output;
}

