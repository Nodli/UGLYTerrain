#pragma once

#include <MultiLayerMap.hpp>
#include <Vegetation/VegetationLayerMap.hpp>

/** \addtogroup Vegetation
 * @{
 */

SimpleLayerMap bush_density(const BiomeInfo& bi);

class Bush : public Plant
{
public:
    Bush(const int ID, const int max_age, const int reproduction_age, const double health, const SimpleLayerMap* density) 
        : Plant(ID, max_age, reproduction_age, health, density) {}
    Bush(const Bush& b) 
        : Bush(b._ID, b._max_age, b._reproduction_age, b._health, b._density) {} 
	virtual bool is_dead() const;
	virtual void update(std::mt19937& gen, std::uniform_real_distribution<>& rdis, VegetationLayerMap& distribution, const int i, const int j);
};


/** @}*/