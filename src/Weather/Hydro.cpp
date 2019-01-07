#include <Weather/Hydro.hpp>
#include <Utils.hpp>
#include <iostream>

void one_way(const DoubleField& heightmap, SimpleLayerMap& area, const std::vector<std::pair<double, Eigen::Vector2i>>& field)
{
	for(int i = 0; i < field.size(); ++i)
	{
		// coordinates of the ith highest cell
		int x = field[i].second.x();
		int y = field[i].second.y();
		// neighbors informations
		double values[8];
		Eigen::Vector2i positions[8];
		double slopes[8];

		int neigh_nb = heightmap.neighbors_info_filter(field[i].second, values, positions, slopes);
		int lowest_neigh = 0;

		// get the lowest neighbor
		for(int j = 0; j < neigh_nb; ++j)
		{
			if(slopes[j] < slopes[lowest_neigh])
			{
				lowest_neigh = j;
			}
		}

		// add value of the ith highest cell to the lowest neighbor
		area.at(positions[lowest_neigh].x(), positions[lowest_neigh].y()) += area.value(x, y);
	}
}

void distribution(const DoubleField& heightmap, SimpleLayerMap& area, const std::vector<std::pair<double, Eigen::Vector2i>>& field)
{
	for(int i = 0; i < field.size(); ++i)
	{
		// coordinates of the ith highest cell
		int x = field[i].second.x();
		int y = field[i].second.y();
		// neighbors informations
		double values[8];
		Eigen::Vector2i positions[8];
		double slopes[8];
		double proportions[8];
		int neigh_nb = heightmap.neighbors_info_filter(field[i].second, values, positions, slopes);
		proportion(neigh_nb, slopes, proportions);

		// add to each neighbor the proportion of the ith highest cell value
		for(int j = 0; j < neigh_nb; ++j)
		{
			area.at(positions[j].x(), positions[j].y()) += area.value(x, y) * (proportions[j]);
		}
	}
}

SimpleLayerMap get_area(const DoubleField& heightmap, bool distribute)
{
	SimpleLayerMap area = SimpleLayerMap(static_cast<Grid2d>(heightmap));
	area.set_all(1.0);

	std::vector<std::pair<double, Eigen::Vector2i>> field = heightmap.sort_by_height();

	if(distribute)	distribution(heightmap, area, field);
	else 						one_way(heightmap, area, field);
	
	return area;
}

SimpleLayerMap get_water_indexes(const DoubleField& heightmap)
{
	SimpleLayerMap area = get_area(heightmap, true);
	SimpleLayerMap slope = SimpleLayerMap::generate_slope_map(heightmap).normalize();
	SimpleLayerMap water_index = SimpleLayerMap(static_cast<Grid2d>(heightmap));

	double k = 4.0;

	for(int j = 0; j < area.grid_height(); ++j)
	{
		for(int i = 0; i < area.grid_width(); i++)
		{
			water_index.set_value(i, j, sqrt(area.value(i, j))/(1+k*heightmap.slope(i, j)));
		}
	}

	return water_index;
}

void erode_from_area(MultiLayerMap& layers, double k, bool distribute)
{
	SimpleLayerMap area = get_area(layers, distribute);
	SimpleLayerMap eroded_quantity(area);
	SimpleLayerMap sed_quantity(area);
	SimpleLayerMap slope = SimpleLayerMap::generate_slope_map(layers);
	slope.normalize();

	for(int j = 0; j < area.grid_height(); ++j)
	{
		for(int i = 0; i < area.grid_width(); i++)
		{
			// weighted by slope: less effect on plains
			// sqrt(slope * sqrt(area))
			eroded_quantity.set_value(i, j, sqrt(slope.value(i, j) * sqrt(area.value(i, j))));
		}
	}

	eroded_quantity.normalize();
	eroded_quantity *= k;
	double eroded_quantity_sum = eroded_quantity.get_sum();

	layers.get_field(0) -= eroded_quantity;

	// add sed
	for(int j = 0; j < area.grid_height(); ++j)
	{
		for(int i = 0; i < area.grid_width(); i++)
		{
			// sqrt(area) / sqrt(1+slope*slope)
			sed_quantity.set_value(i, j, sqrt(area.value(i, j)) / sqrt(1 + slope.value(i, j) * slope.value(i, j)));
		}
	}

	sed_quantity.normalize();
	double sed_quantity_sum = sed_quantity.get_sum();
	sed_quantity = sed_quantity * (1/sed_quantity_sum) * eroded_quantity_sum;

	layers.get_field(1) += sed_quantity;
}

void water_drop_transport(MultiLayerMap& layers, std::mt19937& gen, int n, double water_per_drop, double water_loss, double initial_speed)
{
	std::uniform_int_distribution<> dis_width(0, layers.grid_width() - 1);
	std::uniform_int_distribution<> dis_height(0, layers.grid_width() - 1);

	for(int i = 0; i < n; i++)
	{
		int x = dis_width(gen);
		int y = dis_height(gen);

		double qty_sed = 0.0;
		double qty_water = water_per_drop;
		double speed = initial_speed;

		while(qty_water > 0.0)
		{
			// update qty_sed TODO
			double delta_sed = 0.0;	// to mind: can't have abs(delta_sed) > qty_sed, qty_sed always > 0
			layers.get_field(0).at(x, y) -= delta_sed;
			qty_sed += delta_sed;

			// update speed

			// compute new x, y

			// update qty_water
			qty_water -= water_loss;
		}

		layers.get_field(0).at(x, y) += qty_sed;
	}
}