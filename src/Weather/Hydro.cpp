#include <Weather/Hydro.hpp>
#include <Utils.hpp>
#include <iostream>

SimpleLayerMap get_area(const DoubleField& heightmap, bool distribute)
{
	SimpleLayerMap area = SimpleLayerMap(static_cast<Grid2d>(heightmap));
	area.set_all(1.0);

	std::vector<std::pair<double, Eigen::Vector2i>> field = heightmap.sort_by_height();

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

		if(distribute)
		{
			double proportions[8];
			proportion(neigh_nb, slopes, proportions);

			// add to each neighbor the proportion of the ith highest cell value
			for(int j = 0; j < neigh_nb; ++j)
			{
				area.at(positions[j].x(), positions[j].y()) += area.value(x, y) * (proportions[j]);
			}
		}
		else
		{
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

void erode_from_area(MultiLayerMap& layers, const SimpleLayerMap& area, double k, double kd, bool transport)
{
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

	if(transport)
	{
		// add sed
		for(int j = 0; j < area.grid_height(); ++j)
		{
			for(int i = 0; i < area.grid_width(); i++)
			{
				// sqrt(area) / sqrt(1+slope*slope)
				sed_quantity.set_value(i, j, std::min(sqrt(area.value(i, j)), kd) / sqrt(1 + slope.value(i, j) * slope.value(i, j)));
			}
		}

		sed_quantity.normalize();
		double sed_quantity_sum = sed_quantity.get_sum();
		sed_quantity = sed_quantity * (1/sed_quantity_sum) * eroded_quantity_sum;

		layers.get_field(1) += sed_quantity;
	}
}

void erode_from_droplets(MultiLayerMap& layers, std::mt19937& gen, const SimpleLayerMap& brush, int n, double water_loss, double k, double kd)
{
	std::uniform_int_distribution<> dis_width(0, layers.grid_width() - 1);
	std::uniform_int_distribution<> dis_height(0, layers.grid_width() - 1);
	std::uniform_real_distribution<> dis_proportion(0, 1);

	SimpleLayerMap& firstField = layers.get_field(0);
	SimpleLayerMap heightmap = layers.generate_field();

	for(int i = 0; i < n; i++)
	{
		int x = dis_width(gen);
		int y = dis_height(gen);
		int next_x = 0;
		int next_y = 0;

		double delta_sed = 0.0;
		double qty_sed = 0.0;
		double qty_water = 1.0;

		while(qty_water > 0.0)
		{
			// compute new x, y
			double values[8];
			Eigen::Vector2i positions[8];
			double slopes[8];
			double proportions[8];
			int neigh_nb = heightmap.neighbors_info_filter(Eigen::Vector2i(x, y), values, positions, slopes);

			// try deposit while no lower neighbor
			while(neigh_nb == 0 && qty_sed > 0)
			{
				delta_sed = -0.1 * k;

				if(-delta_sed > qty_sed)			delta_sed = -qty_sed;				// qty_sed >= 0
				if(delta_sed + qty_sed > 1.0) delta_sed = 1.0 - qty_sed;	// qty_sed <= 1

				firstField.at(x, y) -= delta_sed;
				heightmap.at(x, y) -= delta_sed;
				qty_sed += delta_sed;

				neigh_nb = heightmap.neighbors_info_filter(Eigen::Vector2i(x, y), values, positions, slopes);
			}

			// if in a pit
			if(neigh_nb == 0)
			{
				qty_water = 0.0;
			}
			else
			{
				// select the next position
				proportion(neigh_nb, slopes, proportions);
				double p = dis_proportion(gen);
				double proportion_sum = 0.0;

				for(int j = 0; j < neigh_nb; j++)
				{
					if(proportion_sum <= p && proportion_sum + proportions[j] >= p)
					{
						next_x = positions[j].x();
						next_y = positions[j].y();
						break;
					}
					proportion_sum += proportions[j];
				}

				// update speed
				double speed = heightmap.slope(x, y);

				// compute delta_sed
				double max_erode = std::min(heightmap.value(x, y) - heightmap.value(next_x, next_y), 1.0);

				double erode = std::min(speed, max_erode);						      			// between 0 and 1
				double deposit = 1.0 - std::min(std::sqrt(kd + speed), 1.0);	    // between 0 and 1
				delta_sed = erode - deposit;																			// between -1 and 1
				delta_sed *= k;

				if(-delta_sed > qty_sed)			delta_sed = -qty_sed;				// qty_sed >= 0
				if(delta_sed + qty_sed > 1.0) delta_sed = 1.0 - qty_sed;	// qty_sed <= 1

				double unused = 0.0;
				// update qty_sed and layers
				for(int row = 0; row < brush.grid_height(); ++row)
				{
					for(int col = 0; col < brush.grid_width(); ++col)
					{
						int brush_origin = brush.grid_width() >> 1;
						int field_col = x + col - brush_origin;
						int field_row = y + row - brush_origin;

						if(field_col < 0 || field_row < 0 || field_col >= firstField.grid_height() || field_row >= firstField.grid_width())
						{
							unused += brush.value(col, row) * delta_sed;
						}
						else
						{
							firstField.at(field_col, field_row) -= brush.value(col, row) * delta_sed;
							heightmap.at(field_col, field_row) -= brush.value(col, row) * delta_sed;
						}
					}
				}
				
				firstField.at(x, y) -= unused;
				heightmap.at(x, y) -= unused;
				qty_sed += delta_sed;	// qty_sed between 0 and 1

				// update position
				x = next_x;
				y = next_y;

				// update qty_water
				qty_water -= water_loss;
			}
		}

		firstField.at(x, y) += qty_sed;
	}
}