#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>

#include <queue>
#include <iostream>

#include <Weather/Erosion.hpp>
#include <Weather/Biome.hpp>
#include <BooleanField.hpp>
#include <Utils.hpp>

void erode_constant(MultiLayerMap& layers, const double k){
	assert(layers.get_layer_number() > 0);

	// creating the sediment layer if necessary
	if (layers.get_layer_number() == 1){
		layers.new_layer();
	}

	for(int h = 0; h < layers.grid_height(); ++h)
	{
		for(int w = 0; w < layers.grid_width(); ++w)
		{
			layers.get_field(0).at(w, h) -= k; // berock
			layers.get_field(1).at(w, h) += k; // sediments
		}
	}

}

void erode_using_median_slope(MultiLayerMap& layers, const double k){
	assert(layers.get_layer_number() > 0);

	// creating the sediment layer if necessary
	if (layers.get_layer_number() == 1){
		layers.new_layer();
	}

	Eigen::Vector2i positions[8];
	double values[8];
	double slopes[8];

	for(int h = 0; h < layers.grid_height(); ++h){
		for(int w = 0; w < layers.grid_width(); ++w){
			// getting neighbor slope
			int neighbors = layers.neighbors_info(w, h, values, positions, slopes);

			// computing the median slope
			abs_array(neighbors, slopes);
			double median_slope = median_array(neighbors, slopes);

			// applying erosion
			layers.get_field(0).at(w, h) -= k * median_slope;
			layers.get_field(1).at(w, h) += k * median_slope;
		}
	}
}

void erode_using_median_double_slope(MultiLayerMap& layers, const double k){
	assert(layers.get_layer_number() > 0);

	// creating the sediment layer if necessary
	if (layers.get_layer_number() == 1){
		layers.new_layer();
	}

	// 8-connexity double slopes
	// up slope, up-right slope, mid-slope, bottom-right slope
	double slopes[4];

	for(int h = 0; h < layers.grid_height(); ++h){
		for(int w = 0; w < layers.grid_width(); ++w){
			Eigen::Vector2i midpoint = {w, h};

			Eigen::Vector2i A;
			Eigen::Vector2i B;

			// up slope
			A.x() = w;
			A.y() = (h - 1 > 0) ? (h - 1) : (h);
			B.x() = w;
			B.y() = (h + 1 < layers.grid_height() - 1) ? (h + 1) : (h);
			slopes[0] = std::abs(layers.value(B.x(), B.y()) - layers.value(A.x(), A.y())) / 2.;

			// up-right slope
			A.x() = (w + 1 < layers.grid_width() - 1) ? (w + 1) : (w);
			B.x() = (w - 1 > 0) ? (w - 1) : (w);
			slopes[1] = std::abs(layers.value(B.x(), B.y()) - layers.value(A.x(), A.y())) / (2. * std::sqrt(2));

			// mid-slope
			A.y() = h;
			B.y() = h;
			slopes[2] = std::abs(layers.value(B.x(), B.y()) - layers.value(A.x(), A.y())) / 2.;

			// bottom-right slope
			A.y() = (h - 1 > 0) ? (h - 1) : (h);
			B.y() = (h + 1 < layers.grid_height() - 1) ? (h + 1) : (h);
			slopes[3] = std::abs(layers.value(B.x(), B.y()) - layers.value(A.x(), A.y())) / (2. * std::sqrt(2));

			// computing the median slope
			double median_slope = median_array(4, slopes);

			// applying erosion
			layers.get_field(0).at(w, h) -= k * median_slope;
			layers.get_field(1).at(w, h) += k * median_slope;
		}
	}
}

void erode_using_mean_slope(MultiLayerMap& layers, const double k){
	assert(layers.get_layer_number() > 0);

	// creating the sediment layer if necessary
	if (layers.get_layer_number() == 1){
		layers.new_layer();
	}

	Eigen::Vector2i positions[8];
	double values[8];
	double slopes[8];

	for(int h = 0; h < layers.grid_height(); ++h){
		for(int w = 0; w < layers.grid_width(); ++w){
			// getting neighbor slope
			int neighbors = layers.neighbors_info(w, h, values, positions, slopes);

			// computing the median slope
			abs_array(neighbors, slopes);
			double mean_slope = mean_array(neighbors, slopes);

			// applying erosion
			layers.get_field(0).at(w, h) -= k * mean_slope;
			layers.get_field(1).at(w, h) += k * mean_slope;
		}
	}
}

void erode_using_mean_double_slope(MultiLayerMap& layers, const double k){
	assert(layers.get_layer_number() > 0);

	// creating the sediment layer if necessary
	if (layers.get_layer_number() == 1){
		layers.new_layer();
	}

	// 8-connexity double slopes
	// up slope, up-right slope, mid-slope, bottom-right slope
	double slopes[4];

	for(int h = 0; h < layers.grid_height(); ++h){
		for(int w = 0; w < layers.grid_width(); ++w){
			Eigen::Vector2i midpoint = {w, h};

			Eigen::Vector2i A;
			Eigen::Vector2i B;

			// up slope
			A.x() = w;
			A.y() = (h - 1 > 0) ? (h - 1) : (h);
			B.x() = w;
			B.y() = (h + 1 < layers.grid_height() - 1) ? (h + 1) : (h);
			slopes[0] = std::abs(layers.value(B.x(), B.y()) - layers.value(A.x(), A.y())) / 2.;

			// up-right slope
			A.x() = (w + 1 < layers.grid_width() - 1) ? (w + 1) : (w);
			B.x() = (w - 1 > 0) ? (w - 1) : (w);
			slopes[1] = std::abs(layers.value(B.x(), B.y()) - layers.value(A.x(), A.y())) / (2. * std::sqrt(2));

			// mid-slope
			A.y() = h;
			B.y() = h;
			slopes[2] = std::abs(layers.value(B.x(), B.y()) - layers.value(A.x(), A.y())) / 2.;

			// bottom-right slope
			A.y() = (h - 1 > 0) ? (h - 1) : (h);
			B.y() = (h + 1 < layers.grid_height() - 1) ? (h + 1) : (h);
			slopes[3] = std::abs(layers.value(B.x(), B.y()) - layers.value(A.x(), A.y())) / (2. * std::sqrt(2));

			// computing the median slope
			double mean_slope = mean_array(4, slopes);

			// applying erosion
			layers.get_field(0).at(w, h) -= k * mean_slope;
			layers.get_field(1).at(w, h) += k * mean_slope;
		}
	}
}

void erode_using_exposition(MultiLayerMap& layers, const double k){
	assert(layers.get_layer_number() > 0);

	// creating the sediment layer if necessary
	if(layers.get_layer_number() == 1){
		layers.new_layer();
	}

	SimpleLayerMap terrain_exposure = get_light_exposure(layers);
	terrain_exposure.normalize();

	// apply erosion on layers
	for(int h = 0; h < layers.grid_height(); ++h){
		for(int w = 0; w < layers.grid_width(); ++w){
			layers.get_field(0).at(w, h) -= k * terrain_exposure.at(w, h);
			layers.get_field(1).at(w, h) += k * terrain_exposure.at(w, h);
		}
	}
}

void erode_layered_materials_using_exposition(MultiLayerMap& layers,
					const std::vector<double>& layers_top_heights,
					const std::vector<double>& layers_erosion_values){

	assert(layers.get_layer_number() > 0);
	assert(layers_top_heights.size() > 0 && layers_top_heights.size() == layers_erosion_values.size());

	// creating the sediment layer if necessary
	if(layers.get_layer_number() == 1){
		layers.new_layer();
	}

	//NANI DESU KA ?
	SimpleLayerMap terrain_exposure = get_light_exposure(layers);
	terrain_exposure.normalize();

	SimpleLayerMap terrain = layers.generate_field();

	// apply erosion on layers
	for(int h = 0; h < layers.grid_height(); ++h){
		for(int w = 0; w < layers.grid_width(); ++w){

			// find the erosion value based on the height of the current layer
			int ilayer = 1;
			while(ilayer < layers_top_heights.size()
			&& terrain.at(w, h) > layers_top_heights[ilayer]){
				++ilayer;
			}

			double material_erosion_value = layers_erosion_values[ilayer];

			layers.get_field(0).at(w, h) -= material_erosion_value;
			layers.get_field(1).at(w, h) += material_erosion_value;
		}
	}
}

void transport(MultiLayerMap& layers, const double rest_angle, const double quantity_tolerance)
{
	assert(layers.get_layer_number() > 0);

	// the difference in height between two adjacent cells under which the pile is considered stable
	double slope_stability_threshold = layers.cell_size().x() * tan(rest_angle / 180. * 3.14);

	// temp storage of neighborhood
	double values[8];
	Eigen::Vector2i positions[8];
	double slopes[8];

	// generating the base terrain layer on which slopes will be computed
	SimpleLayerMap terrain = layers.generate_field();


	// temporary vector to shuffle grid cells
	std::vector<Eigen::Vector2i> coord_vector;
	for(int i = 0; i < terrain.grid_height(); ++i){
		for(int j = 0; j < terrain.grid_width(); ++j){
			coord_vector.push_back({i, j});
		}
	}
	std::random_shuffle(coord_vector.begin(), coord_vector.end());

	// queue all cells of the grid as they could be unstable
	std::queue<Eigen::Vector2i> unstable_coord;
	for(int i = 0; i != coord_vector.size(); ++i){
		unstable_coord.push(coord_vector[i]);
	}

	coord_vector.clear();

	// updating stability map with all cells unstable
	BooleanField stability_map(layers.grid_width(), layers.grid_height(), false);

	std::cout << "slope_stability_threshold: " << slope_stability_threshold << std::endl;

	while(!unstable_coord.empty()){
		std::cout << "queue size: " << unstable_coord.size() << std::endl;

		// pick the next unstable cell
		const Eigen::Vector2i& unstable_cell = unstable_coord.front();

		int counter = 0;

		int neighbors;
		bool available_sediments = true;

		do{
			// checking that there is a relevant amount of sediments at the cell
			if(layers.get_field(1).at(unstable_cell) < quantity_tolerance){
				break;
			}

			//computing neighborhood parameters
			neighbors = terrain.neighbors_info_filter(unstable_cell, values, positions, slopes,
								  - slope_stability_threshold, false);

			if(neighbors > 0){
				opp_array(neighbors, slopes); // values are all negative here

				// stabilization
				double min_neighborhood_slope = min_array(neighbors, slopes);
				double sediments_at_unstable_cell = layers.get_field(1).at(unstable_cell);

				// minimal amount of sediments missing to stabilize unstable_cell
				// with regard to the easiest neighbor (the highest among unstable neighbors)
				// sub. in this order because there must be min_neighborhood_slope > slope_stability_threshold
				double min_stability_difference = min_neighborhood_slope - slope_stability_threshold;

				// minimal amount of sediments to transport to all neighbors to stabilize unstable_cell
				// with regard to the neighbor that gave min_neighborhood_slope
				// @DEBUG: not sure, does the matter need to be ponderated by the 8-connexity distance or this is considered done when ponderating the slope
				double min_stability_all_neighbors = min_stability_difference * neighbors;
				double amount_to_transport_all_neighbors = min_stability_difference * neighbors;
				if(amount_to_transport_all_neighbors > sediments_at_unstable_cell){
					amount_to_transport_all_neighbors = sediments_at_unstable_cell;
					available_sediments = false;
				}

				// checking that amount_to_transport is a relevant amount of sediments
				double amount_to_transport = amount_to_transport_all_neighbors / neighbors;
				if(amount_to_transport < quantity_tolerance){
					break;
				}

				// transporting some sediments to stabilize with regard to one neighbor
				for(int neigh = 0; neigh != neighbors; ++neigh){
					// updating the sediment layer
					layers.get_field(1).at(unstable_cell) -= amount_to_transport;
					layers.get_field(1).at(positions[neigh]) += amount_to_transport;

					// updating terrain
					terrain.at(unstable_cell) -= amount_to_transport;
					terrain.at(positions[neigh]) += amount_to_transport;

					// adding neighbor to queue as if it may have become unstable
					if(stability_map.at(positions[neigh])){
						stability_map.at(positions[neigh]) = false;
						unstable_coord.push(positions[neigh]);
					}
				}
			}
		}while(neighbors > 0 && available_sediments);

		// unstable_cell is now stable either because the slope difference is not big enough anymore
		// or because there is no more sediments to transport from unstable_cell
		stability_map.at(unstable_cell) = true;
		unstable_coord.pop();
	}
}

void transport_4connex(MultiLayerMap& layers, const double rest_angle, const double quantity_tolerance)
{
	assert(layers.get_layer_number() > 0);

	// the difference in height between two adjacent cells under which the pile is considered stable
	double slope_stability_threshold = layers.cell_size().x() * tan(rest_angle / 180. * 3.14);

	// temp storage of neighborhood
	double values[8];
	Eigen::Vector2i positions[8];
	double slopes[8];

	// generating the base terrain layer on which slopes will be computed
	SimpleLayerMap terrain = layers.generate_field();

	BooleanField stability_map(layers.grid_width(), layers.grid_height(), false);

	// queue all cells of the grid as they could be unstable
	// @DEBUG: using a temp vector to randomize the order
	std::vector<Eigen::Vector2i> coord_vector;
	for(int i = 0; i < terrain.grid_height(); ++i){
		for(int j = 0; j < terrain.grid_width(); ++j){
			coord_vector.push_back({i, j});
		}
	}
	std::random_shuffle(coord_vector.begin(), coord_vector.end());
	std::queue<Eigen::Vector2i> unstable_coord;
	for(int i = 0; i != coord_vector.size(); ++i){
		unstable_coord.push(coord_vector[i]);
	}

	std::cout << "slope_stability_threshold: " << slope_stability_threshold << std::endl;

	while(!unstable_coord.empty()){
		std::cout << "queue size: " << unstable_coord.size() << std::endl;

		// pick the next unstable cell
		const Eigen::Vector2i& unstable_cell = unstable_coord.front();

		int counter = 0;

		int neighbors;
		bool available_sediments = true;

		do{
			// checking that there is a relevant amount of sediments at the cell
			if(layers.get_field(1).at(unstable_cell) < quantity_tolerance){
				break;
			}

			//computing neighborhood parameters
			neighbors = terrain.neighbors_info_filter_4connex(unstable_cell, values, positions, slopes,
								  - slope_stability_threshold, false);

			if(neighbors > 0){
				opp_array(neighbors, slopes); // values are all negative here

				// stabilization
				double min_neighborhood_slope = min_array(neighbors, slopes);
				double sediments_at_unstable_cell = layers.get_field(1).at(unstable_cell);

				// minimal amount of sediments missing to stabilize unstable_cell
				// with regard to one of its neighbor
				// sub. in this order because there must be min_neighborhood_slope > slope_stability_threshold
				double min_stability_difference = min_neighborhood_slope - slope_stability_threshold;

				// minimal amount of sediments to transport to all neighbors to stabilize unstable_cell
				// with regard to the neighbor that gave min_neighborhood_slope
				// @DEBUG: not sure, does the matter need to be ponderated by the 8-connexity distance or this is considered done when ponderating the slope
				double min_stability_all_neighbors = min_stability_difference * neighbors;
				double amount_to_transport_all_neighbors = min_stability_difference * neighbors;
				if(amount_to_transport_all_neighbors > sediments_at_unstable_cell){
					amount_to_transport_all_neighbors = sediments_at_unstable_cell;
					available_sediments = false;
				}

				// checking that amount_to_transport is a relevant amount of sediments
				double amount_to_transport = amount_to_transport_all_neighbors / neighbors;
				if(amount_to_transport < quantity_tolerance){
					break;
				}

				// transporting some sediments to stabilize with regard to one neighbor
				for(int neigh = 0; neigh != neighbors; ++neigh){
					// updating the sediment layer
					layers.get_field(1).at(unstable_cell) -= amount_to_transport;
					layers.get_field(1).at(positions[neigh]) += amount_to_transport;

					// updating terrain
					terrain.at(unstable_cell) -= amount_to_transport;
					terrain.at(positions[neigh]) += amount_to_transport;

					// adding neighbor to queue as if it may have become unstable
					if(stability_map.at(positions[neigh])){
						stability_map.at(positions[neigh]) = false;
						unstable_coord.push(positions[neigh]);
					}
				}
			}
		}while(neighbors > 0 && available_sediments);

		// unstable_cell is now stable either because the slope difference is not big enough anymore
		// or because there is no more sediments to transport from unstable_cell
		stability_map.at(unstable_cell) = true;
		unstable_coord.pop();
	}
}
