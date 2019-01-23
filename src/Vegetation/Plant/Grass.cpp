#include <Vegetation/Plant/Grass.hpp>

SimpleLayerMap low_grass_density(const BiomeInfo& bi)
{
	SimpleLayerMap density(static_cast<Grid2d>(bi.height));

	for(int j = 0; j < density.grid_height(); ++j)
	{
		for(int i = 0; i < density.grid_width(); i++)
		{
			double value = std::min(1.5*sqrt(bi.water_index.value(i, j)) * (1 - bi.height.value(i, j)), 1.0);
			density.set_value(i, j, value);
		}
	}

	return density;
}

SimpleLayerMap strong_grass_density(const BiomeInfo& bi)
{
	SimpleLayerMap density(static_cast<Grid2d>(bi.height));

	for(int j = 0; j < density.grid_height(); ++j)
	{
		for(int i = 0; i < density.grid_width(); i++)
		{
			double value = std::min(1.3*std::max(sqrt(bi.water_index.value(i, j)), bi.sediments.value(i, j)*bi.sediments.value(i, j)), 1.0);
			density.set_value(i, j, value);
		}
	}

	return density;
}

bool Grass::is_dead() const
{
	return _age > _max_age * sqrt(_health) * 1.2;
}

void Grass::update(std::mt19937& gen, std::uniform_real_distribution<>& rdis, VegetationLayerMap& distribution, const int i, const int j)
{
	_age++;

	if(_age > _reproduction_age)
	{
		Eigen::Vector2i pos[8];
		float rep = rdis(gen);
		float chance = rdis(gen);

		if(rep < 1)
		{
			int nb = distribution.neighbors(i, j, pos);
			float vn = rdis(gen);
			int select_nei = vn * (nb - 1);
			auto& target = distribution.at(pos[select_nei].x(), pos[select_nei].y());

			if(target.size() < 10 && chance < _density->value(pos[select_nei]))
			{
				target.push_back(new Grass(_ID, _max_age, _reproduction_age, _density->value(pos[select_nei]), _density));
			}
		}
	}
}