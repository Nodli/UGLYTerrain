#pragma once

#include <MultiLayerMap.hpp>
#include <Vegetation/VegetationLayerMap.hpp>
/** \addtogroup Vegetation
 * @{
 */
SimpleLayerMap low_grass_density(const BiomeInfo& bi);
SimpleLayerMap strong_grass_density(const BiomeInfo& bi);

class Grass : public Plant
{
public:
    Grass(const int ID, const int max_age, const int reproduction_age, const double health, const SimpleLayerMap* density) 
        : Plant(ID, max_age, reproduction_age, health, density) {}
    Grass(const Grass& g) 
        : Grass(g._ID, g._max_age, g._reproduction_age, g._health, g._density) {} 
	virtual bool is_dead() const;
	virtual void update(std::mt19937& gen, std::uniform_real_distribution<>& rdis, VegetationLayerMap& distribution, const int i, const int j);
};


/** @}*/