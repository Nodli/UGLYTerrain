#include <Weather/Hydro.hpp>

ScalarField get_area(MultiLayerMap& layers)
{
	ScalarField heightMap = layers.generate_field();
	ScalarField area = heightMap;
	area.set_all(1.0);

	std::vector<std::pair<double, Eigen::Vector2i>> field = heightMap.export_to_list();
	std::sort(field.begin(), field.end(), [](const std::pair<double, Eigen::Vector2i>& a, const std::pair<double, Eigen::Vector2i>& b)
	{
		return a.first > b.first;
	});

	one_way(area, field);

	return area;
}

void one_way(ScalarField& area, std::vector<std::pair<double, Eigen::Vector2i>>& field)
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

		int neigh_nb = area.neighbors_info(field[i].second, values, positions, slopes);

		int lowest_neigh = 0;

		// get the lowest neighbor
		for(int j = 0; j < neigh_nb; ++j)
		{
			if(slopes[j] < slopes[lowest_neigh]) lowest_neigh = j;
		}

		// add value of the ith highest cell to the lowest neighbor
		area.at(positions[lowest_neigh].x(), positions[lowest_neigh].y()) += area.value(x, y);
	}
}

void repartition(ScalarField& area, std::vector<std::pair<double, Eigen::Vector2i>>& field)
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

		int neigh_nb = area.neighbors_info(field[i].second, values, positions, slopes);

		// get the lowest neighbor
		for(int j = 0; j < neigh_nb; ++j)
		{
			// if(slopes[j] < slopes[lowest_neigh]) lowest_neigh = j;
		}

		// add value of the ith highest cell to the lowest neighbor
		// area.at(positions[lowest_neigh].x(), positions[lowest_neigh].y()) += area.value(x, y);
	}
}