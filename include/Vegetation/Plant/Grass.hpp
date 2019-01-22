#pragma once

#include <MultiLayerMap.hpp>
#include <Vegetation/VegetationLayerMap.hpp>

SimpleLayerMap low_grass_density(const BiomeInfo& bi);
SimpleLayerMap strong_grass_density(const BiomeInfo& bi);

class Grass : public Plant
{
public:
    Grass(const int max_age, const int reproduction_age, const double health, const SimpleLayerMap* density) 
        : Plant(max_age, reproduction_age, health, density) {}
    Grass(const Grass& g) 
        : Grass(g._max_age, g._reproduction_age, g._health, g._density) {} 
	virtual bool is_dead();
	virtual void update(std::mt19937& gen, std::uniform_real_distribution<>& rdis, VegetationLayerMap& distribution, const int i, const int j);
};