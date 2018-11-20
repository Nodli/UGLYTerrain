#include <Weather/Erosion.hpp>

void erode(MultiLayerMap& layers, const double k)
{
	for(int j = 0; j < layers.grid_height(); ++j)
	{
		for(int i = 0; i < layers.grid_width(); ++i)
		{
			layers.get_field(0).at(i, j) -= k * layers.get_field(0).slope(i, j);
		}
	}
}

void erode_and_transport(MultiLayerMap& layers, const double k, const int iteration_max, const double rest_angle)
{
	std::vector<std::pair<double, Eigen::Vector2i>> field;

	if(layers.get_layer_number() == 1)
	{
		layers.new_field();
	}

	if(iteration_max > 0)
	{
		field = layers.get_field(0).export_to_list();
		std::sort(field.begin(), field.end(), [](const std::pair<double, Eigen::Vector2i>& a, const std::pair<double, Eigen::Vector2i>& b)
		{
			return a.first > b.first;
		});
	}

	for(int j = 0; j < layers.grid_height(); ++j)
	{
		for(int i = 0; i < layers.grid_width(); ++i)
		{
			double eroded_quantity = k * layers.get_field(0).slope(i, j);
			layers.get_field(0).at(i, j) -= eroded_quantity;
			layers.get_field(1).at(i, j) += eroded_quantity;
		}
	}

	double values[8];
	Eigen::Vector2i positions[8];
	double slopes[8];

	for(int s = 0; s < iteration_max; ++s)
	{
		ScalarField terrain = layers.generate_field();

		for(auto current_it = field.begin(); current_it != field.end(); current_it--)
		{
			Eigen::Vector2i p_current = current_it->second;
			int cv = terrain.value(p_current);
			int nb = terrain.neighbors_info(p_current, values, positions, slopes);

			for(int c = 0; c < nb; ++c)
			{
				if(cv > values[c])
				{
					///Stuck actually don't understand what I am doing anymore
				}
			}
		}
	}
}