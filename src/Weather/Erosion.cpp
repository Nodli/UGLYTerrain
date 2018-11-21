#include <queue>
#include <Weather/Erosion.hpp>
#include <BooleanField.hpp>
#include <Utils.hpp>


void erode_slope_constant(MultiLayerMap& layers, const double k)
{
	for(int j = 0; j < layers.grid_height(); ++j)
	{
		for(int i = 0; i < layers.grid_width(); ++i)
		{
			double delta_value = k * layers.get_field(0).slope(i, j);

			// removing from bedrock
			layers.get_field(0).at(i, j) -= delta_value;

			// adding to sediments
			if(layers.get_layer_number() > 0){
				layers.get_field(1).at(i, j) += delta_value;
			}

		}
	}
}

float erosion_control_function(const double slope){

	// max slope to consider
	float max_slope_threshold = 40.;

	// fall-off function taken from implicit function ray-marching
	float slope_remap = slope / max_slope_threshold;
	float temp_value = 1. - slope_remap * slope_remap;

	return temp_value * temp_value * temp_value;

}

void erode_slope_controled(MultiLayerMap& layers, const double k){

	for(int j = 0; j < layers.grid_height(); ++j){
		for(int i = 0; i < layers.grid_width(); ++i){

			double delta_value = k * erosion_control_function(layers.get_field(0).slope(i, j));

			layers.get_field(0).at(i, j) -= delta_value;

			if(layers.get_layer_number() > 0){
				layers.get_field(1).at(i, j) += delta_value;
			}
		}
	}

}

void erode_and_transport(MultiLayerMap& layers, const double k, const int iteration_max, const double rest_angle)
{
	// creating erosion layer if needed
	if(layers.get_layer_number() == 1)
	{
		layers.new_field();
	}

	erode_slope_constant(layers, k);

	double values[8];
	Eigen::Vector2i positions[8];
	double slopes[8];
	double slopes_proportions[8];

	for(int s = 0; s < iteration_max; ++s)
	{
		ScalarField terrain = layers.generate_field();

		// all cells are considered unstable at initialization
		std::queue<Eigen::Vector2i> unstable_coord;
		for(int j = 0; j < terrain.grid_width(); ++j){
			for(int i = 0; i < terrain.grid_height(); ++i){
				unstable_coord.emplace(i, j);
			}
		}

		BooleanField stability(layers.grid_width(), layers.grid_height(), true);

		while(!unstable_coord.empty()){

			// stabilize the cell at the begining of the queue
			Eigen::Vector2i unstable_cell = unstable_coord.pop();

			int neighbors = neighbors_info_filter(unstable_cell, values, positions, slopes, 0., false);
			proportion(neighbors, slopes, slopes_proportions);



			unstable_coord.push();

		}


		/*
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
		*/
	}
}