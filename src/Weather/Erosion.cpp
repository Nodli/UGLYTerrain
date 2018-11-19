#include <Weather/Erosion.hpp>

void erode(MultiLayerMap& layers, double k)
{
	for(int j = 0; j < layers.grid_height(); ++j)
	{
		for(int i = 0; i < layers.grid_width(); ++i)
		{
			layers.get_field(0).at(i, j) -= k * layers.get_field(0).slope(i, j);
		}
	}
}

void erode_and_create(MultiLayerMap& layers, double k)
{
	if(layers.get_layer_number() == 1)
	{
		layers.new_field();

		for(int j = 0; j < layers.grid_height(); ++j)
		{
			for(int i = 0; i < layers.grid_width(); ++i)
			{
				double eroded_quantity = k * layers.get_field(0).slope(i, j);
				layers.get_field(0).at(i, j) -= eroded_quantity;
				layers.get_field(1).at(i, j) += eroded_quantity;
			}
		}
	}
}