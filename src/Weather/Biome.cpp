#include <Weather/Biome.hpp>

BiomeInfo::BiomeInfo(const MultiLayerMap& m)
	: slope(SimpleLayerMap::generate_slope_map(m).normalize())
	, exposure(get_light_exposure(m).normalize())
	, water_index(get_water_indexes(m).normalize())
	, height(m.generate_field().normalize())
	, sediments(m.get_field(1)) 
	{
		sediments.normalize();
	}

SimpleLayerMap get_light_exposure(const DoubleField& df, const int nb_steps, const int nb_samples)
{
	SimpleLayerMap res(static_cast<Grid2d>(df));
	double total = nb_samples * 3.1415 / 2.0;

	for(int j = 0; j < res.grid_height(); ++j)
	{
		for(int i = 0; i < res.grid_width(); ++i)
		{
			double val = df.value_safe(i, j);
			double sum_exp = 0;

			for(int d = 0; d < nb_samples; d++)
			{
				double angle = (2.*3.1415) * d / nb_samples;
				Eigen::Vector2d delta_pos = {cos(angle), sin(angle)};
				double covA = 0;
				double h = 0;

				for(int s = 0; s < nb_steps; ++s)
				{
					double v = df.value_safe(i + s * delta_pos(0), j + s * delta_pos(1)) - val;

					if(v > 0)
					{
						double tmpCov = atan(v / delta_pos.norm());

						if(tmpCov > covA)
						{
							covA = tmpCov;
						}
					}
				}

				sum_exp += (3.1415 / 2.0) - covA;
			}

			res.at(i, j) = sum_exp / total;
		}
	}

	return res;
}

void save_colorized(const MultiLayerMap& mlm)
{
	double snow_height = 15;
	double sediment_height = 0.01;

	SimpleLayerMap water_index = get_water_indexes(mlm).normalize();
	SimpleLayerMap snow_proba = mlm.generate_field().normalize();
	SimpleLayerMap slope = SimpleLayerMap::generate_slope_map(mlm).normalize();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> noise(0,15);
	std::uniform_real_distribution<> snow(0,1);

	std::string filename = "Terrain_texture.ppm";
	std::ofstream output(filename, std::ofstream::out);
	output << "P3" << std::endl;
	output << mlm.grid_width() << " " << mlm.grid_height() << std::endl;
	output << 255 << std::endl;

	for(int j = mlm.grid_height() - 1; j >= 0; --j)
	{
		for(int i = 0; i < mlm.grid_width(); ++i)
		{
			int val_noise = noise(gen);
			double sn = std::max(snow_proba.value(i, j)-0.2, 0.0);
			double sn_prob = 0.7*std::atan(sn*sn*sn*10-1.6);
			sn_prob = 5.0*std::atan(sn*sn*sn*sn*1.0-0.0);
			if(snow(gen) < sn_prob*(1-slope.value(i, j)))
			{
				output << (235+val_noise) << " " << (235+val_noise) << " " << (235+val_noise) << " ";
			}
			else if(mlm.get_field(1).value(i, j) >= sediment_height)
			{
				int water_val = -25*water_index.value(i, j);
				output << (int)(120+val_noise+water_val) << " " << (int)(65+val_noise+water_val) << " " << (0+val_noise) << " ";
			}
			else
			{
				output << (150+val_noise) << " " << (150+val_noise) << " " << (150+val_noise) << " ";
			}
		}

		output << std::endl;
	}

	output.close();
}