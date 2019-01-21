#include <Vegetation/Vegetation.hpp>

#include <iostream>

SimpleLayerMap grass_density(const MultiLayerMap& m)
{
	SimpleLayerMap slope = SimpleLayerMap::generate_slope_map(m).normalize();
	//SimpleLayerMap exposition = get_light_exposure(m).normalize();
	SimpleLayerMap water_index = get_water_indexes(m).normalize();
	SimpleLayerMap height = m.generate_field().normalize();
	SimpleLayerMap density(static_cast<Grid2d>(m));

	for(int j = 0; j < density.grid_height(); ++j)
	{
		for(int i = 0; i < density.grid_width(); i++)
		{
			double value = sqrt(water_index.value(i, j)) * (1 - height.value(i, j));
			density.set_value(i, j, value);
		}
	}

	return density;
}

SimpleLayerMap bush_density(const MultiLayerMap& m)
{
	SimpleLayerMap slope = SimpleLayerMap::generate_slope_map(m).normalize();
	SimpleLayerMap exposition = get_light_exposure(m).normalize();
	SimpleLayerMap water_index = get_water_indexes(m).normalize();
	SimpleLayerMap height = m.generate_field().normalize();
	SimpleLayerMap density(static_cast<Grid2d>(m));

	for(int j = 0; j < density.grid_height(); ++j)
	{
		for(int i = 0; i < density.grid_width(); i++)
		{
			double value = slope.value(i, j) * height.value(i, j) * exposition.value(i, j) * exposition.value(i, j);
			density.set_value(i, j, value);
		}
	}

	return density;
}

SimpleLayerMap tree_density(const MultiLayerMap& m)
{
	SimpleLayerMap slope = SimpleLayerMap::generate_slope_map(m).normalize();
	SimpleLayerMap exposition = get_light_exposure(m).normalize();
	SimpleLayerMap water_index = get_water_indexes(m).normalize();
	SimpleLayerMap height = m.generate_field().normalize();
	SimpleLayerMap density(static_cast<Grid2d>(m));

	for(int j = 0; j < density.grid_height(); ++j)
	{
		for(int i = 0; i < density.grid_width(); i++)
		{
			double value = water_index.value(i, j) * exposition.value(i, j);
			density.set_value(i, j, value);
		}
	}

	return density;
}

void generate_distribution(const MultiLayerMap& m)
{
	int maxVal = 255;
	SimpleLayerMap distrib(static_cast<Grid2d>(m));
	distrib.set_all(0.0);
	MultiLayerMap distribs(static_cast<Grid2d>(m));
	distribs.add_field(grass_density(m));
	distribs.add_field(bush_density(m));
	distribs.add_field(tree_density(m));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis_width(0, m.grid_width() - 1);
	std::uniform_int_distribution<> dis_height(0, m.grid_width() - 1);
	std::uniform_real_distribution<> rdis(0, 1);
	bool nope = true;

	for(int i = 0; i < 15000; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < distribs.get_field(0).value(x, y))
			{
				distrib.set_value(x, y, 1.0);
				nope = false;
			}
		}
		while(nope);
	}

	for(int i = 0; i < 4000; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < distribs.get_field(1).value(x, y))
			{
				distrib.set_value(x, y, 2.0);
				nope = false;
			}
		}
		while(nope);
	}

	for(int i = 0; i < 4000; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < distribs.get_field(2).value(x, y))
			{
				distrib.set_value(x, y, 3.0);
				nope = false;
			}
		}
		while(nope);
	}

	distrib.export_as_pgm("TEST.pgm", true);
	std::string filename = "distribT.ppm";
	std::ofstream output(filename, std::ofstream::out);
	output << "P3" << std::endl;
	output << distrib.grid_width() << " " << distrib.grid_height() << std::endl;
	output << maxVal << std::endl;

	for(int j = distrib.grid_height() - 1; j >= 0; --j)
	{
		for(int i = 0; i < distrib.grid_width(); ++i)
		{
			if(distrib.value(i, j) == 0)
			{
				output << 0 << " " << 0 << " " << 0 << " ";
			}
			else if(distrib.value(i, j) == 1)
			{
				output << 30 << " " << 255 << " " << 10 << " ";
			}
			else if(distrib.value(i, j) == 2)
			{
				output << 10 << " " << 15 << " " << 200 << " ";
			}
			else if(distrib.value(i, j) == 3)
			{
				output << 255 << " " << 100 << " " << 100 << " ";
			}
		}

		output << std::endl;
	}
	output.close();
}

void save_simulation(VegetationLayerMap& distribution, int iter)
{
	std::string filename = "simulation_" + std::to_string(iter) + ".ppm";
	std::ofstream output(filename, std::ofstream::out);
	output << "P3" << std::endl;
	output << distribution.grid_width() << " " << distribution.grid_height() << std::endl;
	output << 255 << std::endl;

	for(int j = distribution.grid_height() - 1; j >= 0; --j)
	{
		for(int i = 0; i < distribution.grid_width(); ++i)
		{
			output << distribution.at(i, j).size() << " " << 0 << " " << 0 << " ";
			if(distribution.at(i, j).size() != 0)
			std::cout << distribution.at(i, j).size() << " ";
			// if(distrib.value(i, j) == 0)
			// {
			// 	output << 0 << " " << 0 << " " << 0 << " ";
			// }
		}
		std::cout << std::endl;
		output << std::endl;
	}

	output.close();
}

void simulate(const MultiLayerMap& mlm)
{
	SimpleLayerMap slope = SimpleLayerMap::generate_slope_map(mlm).normalize();
	SimpleLayerMap exposition = get_light_exposure(mlm).normalize();
	SimpleLayerMap water_index = get_water_indexes(mlm).normalize();
	SimpleLayerMap height = SimpleLayerMap(mlm).normalize();
	VegetationLayerMap distribution(static_cast<Grid2d>(mlm));
	SimpleLayerMap g_density = grass_density(mlm);
	g_density.export_as_pgm("HOLOLOLOLO.pgm");
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis_width(0, mlm.grid_width() - 1);
	std::uniform_int_distribution<> dis_height(0, mlm.grid_width() - 1);
	std::uniform_real_distribution<> rdis(0, 1);
	bool nope = true;
	Plant ref;
	ref._age = 0;
	ref._max_age = 70;
	ref._reproduction_age = 20;
	ref._health = 1.0;
	Plant p[3];
	p[0] = ref;
	p[1] = p[0];
	p[2] = p[0];

	for(int i = 0; i < 100; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < g_density.value(x, y))
			{
				distribution.at(x, y).push_back(ref);
				nope = false;
			}
		}
		while(nope);
	}

	save_simulation(distribution, 0);
	
	for(int it = 1; it <= 200; ++it)
	{
		for(int j = 0; j < distribution.grid_height(); ++j)
		{
			for(int i = 0; i < distribution.grid_width(); i++)
			{
				std::vector<Plant>& cell = distribution.at(i, j);

				for(auto p = cell.begin(); p != cell.end(); ++p)
				{
					//Plant& pl = cell.at(p);
					//Augmente l'âge
					p->_age += 1;

					if(p->_age > p->_max_age)
					{
						cell.erase(p);
					}
					else if(p->_age > p->_reproduction_age)
					{
						Eigen::Vector2i pos[8];
						int nb = distribution.neighbors(i, j, pos);
						float rep = rdis(gen);

						if(rep < 0.3)
						{
							float vn = rdis(gen);
							int select_nei = vn * nb;
							distribution.at(pos[select_nei].x(), pos[select_nei].y()).push_back(Plant());
						}
					}
				}
			}
		}
		save_simulation(distribution, it);
	}
}