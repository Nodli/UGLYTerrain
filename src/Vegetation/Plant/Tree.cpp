#include <Vegetation/Plant/Tree.hpp>

SimpleLayerMap tree_density(const BiomeInfo& bi)
{
	SimpleLayerMap density(static_cast<Grid2d>(bi.height));

	for(int j = 0; j < density.grid_height(); ++j)
	{
		for(int i = 0; i < density.grid_width(); i++)
		{
			double value = std::max(0.0, bi.exposure.value(i, j)*bi.exposure.value(i, j) * (1-bi.slope.value(i, j)) -0.2);
			density.set_value(i, j, value);
		}
	}

	return density;
}

bool Tree::is_dead() const
{
	return _age > _max_age * sqrt(_health) * 1.5;
}

void Tree::update(std::mt19937& gen, std::uniform_real_distribution<>& rdis, VegetationLayerMap& distribution, const int i, const int j)
{
    static int propagation_radius = 50;
	_age++;

	if(_age >= _reproduction_age)
	{
		float rep = rdis(gen);
		float chance = rdis(gen);

		if(rep < 0.2)
		{

			double angle = (2.*3.1415) * rdis(gen);
            double dist = propagation_radius*rdis(gen);
            Eigen::Vector2i new_pos = {i + dist*cos(angle), j + dist*sin(angle)};
			
            if(new_pos.x() != i && new_pos.y() != j && distribution.inside(new_pos))
            {
                auto& target = distribution.at(new_pos.x(), new_pos.y());

                if(target.size() < 5 && chance < _density->value(new_pos))
                {
                    target.push_back(new Tree(_ID, _max_age, _reproduction_age, _density->value(new_pos), _density));
                    // target.push_back(new Grass(Grass::BASE_ID, 40, 10, 1.0, _density));
                    // target.push_back(new Grass(Grass::BASE_ID, 40, 10, 1.0, _density));
                    // target.push_back(new Grass(Grass::BASE_ID, 40, 10, 1.0, _density));
                    // target.push_back(new Grass(Grass::BASE_ID, 40, 10, 1.0, _density));
                    // target.push_back(new Grass(Grass::BASE_ID, 40, 10, 1.0, _density));
                }
            }
		}
	}
}