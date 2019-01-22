#include <Vegetation/Vegetation.hpp>

#include <iostream>

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
	BiomeInfo bi(m);
	int maxVal = 255;
	SimpleLayerMap distrib(static_cast<Grid2d>(m));
	distrib.set_all(0.0);
	MultiLayerMap distribs(static_cast<Grid2d>(m));
	distribs.add_field(low_grass_density(bi));
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
	std::string filename = "Simu_3/simulation_" + std::to_string(iter) + ".ppm";
	std::ofstream output(filename, std::ofstream::out);
	output << "P3" << std::endl;
	output << distribution.grid_width() << " " << distribution.grid_height() << std::endl;
	output << 20 << std::endl;

	for(int j = distribution.grid_height() - 1; j >= 0; --j)
	{
		for(int i = 0; i < distribution.grid_width(); ++i)
		{
			int size = distribution.at(i, j).size();
			// if(size > 0)
			// output << 0 << " " << 0 << " " << 0 << " ";
			// else
			output << 25-size << " " << 25 << " " << 25-size << " ";
			// else
			// output << 25 << " " << 25 << " " << 25 << " ";
			// if(distribution.at(i, j).size() != 0)
			// std::cout << distribution.at(i, j).size() << " ";
			// if(distrib.value(i, j) == 0)
			// {
			// 	output << 0 << " " << 0 << " " << 0 << " ";
			// }
		}
		//std::cout << std::endl;
		output << std::endl;
	}

	output.close();
}

void simulate(const MultiLayerMap& mlm)
{
	// SimpleLayerMap slope = SimpleLayerMap::generate_slope_map(mlm).normalize();
	// SimpleLayerMap exposition = get_light_exposure(mlm).normalize();
	// SimpleLayerMap water_index = get_water_indexes(mlm).normalize();
	// SimpleLayerMap height = SimpleLayerMap(mlm).normalize();
	BiomeInfo bi(mlm);
	VegetationLayerMap distribution(static_cast<Grid2d>(mlm));
	SimpleLayerMap g_density = strong_grass_density(mlm);
	g_density.export_as_pgm("HOLOLOLOLO2.pgm");
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis_width(0, mlm.grid_width() - 1);
	std::uniform_int_distribution<> dis_height(0, mlm.grid_width() - 1);
	std::uniform_real_distribution<> rdis(0, 1);
	bool nope = true;
	Grass ref(50, 10, 1.0, &g_density);
	// ref._age = 0;
	// ref._max_age = 50;
	// ref._reproduction_age = 10;
	// ref._health = 1.0;
	// Plant p[3];
	// p[0] = ref;
	// p[1] = p[0];
	// p[2] = p[0];

	for(int i = 0; i < 10; ++i)
	{
		nope = true;

		do
		{
			int x = dis_width(gen);
			int y = dis_height(gen);

			if(rdis(gen) < g_density.value(x, y))
			{
				distribution.at(x, y).push_back(new Grass(ref));
				nope = false;
			}
		}
		while(nope);
	}

	save_simulation(distribution, 0);
	
	for(int it = 1; it <= 500; ++it)
	{
		for(int j = 0; j < distribution.grid_height(); ++j)
		{
			for(int i = 0; i < distribution.grid_width(); i++)
			{
				std::vector<Plant*>& cell = distribution.at(i, j);

				auto p = cell.begin();
				//for(auto p = cell.begin(); p != cell.end(); ++p)
				while(p != cell.end())
				{
					(*p)->update(gen, rdis, distribution, i, j);
					if((*p)->is_dead())
					{
						delete *p;
						p = cell.erase(p);
					}
					else
					{
						p++;
					}
					//Plant& pl = cell.at(p);
					//Augmente l'âge
					// p->_age += 1;

					// if(p->_age > p->_max_age*sqrt(p->_health)*2)
					// {
					// 	p = cell.erase(p);
					// }
					// else if(p->_age > p->_reproduction_age)
					// {
					// 	Eigen::Vector2i pos[8];
					// 	float rep = rdis(gen);
					// 	float chance = rdis(gen);

					// 	if(rep < 1)
					// 	{
					// 		int nb = distribution.neighbors(i, j, pos);
					// 		float vn = rdis(gen);
					// 		int select_nei = vn * (nb-1);
					// 		auto& target = distribution.at(pos[select_nei].x(), pos[select_nei].y());
					// 		if(target.size() < 20 && chance < g_density.value(pos[select_nei])){
					// 			ref._health = g_density.value(pos[select_nei]);
					// 			target.push_back(ref);
					// 		}
					// 	}
					// 	p++;
					// }
				}
			}
		}
		if(it%10 == 0)
		save_simulation(distribution, it);
	}
}