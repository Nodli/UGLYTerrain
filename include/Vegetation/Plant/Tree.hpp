#pragma once

#include <MultiLayerMap.hpp>
#include <Vegetation/VegetationLayerMap.hpp>
#include <Vegetation/Plant/Grass.hpp>

SimpleLayerMap tree_density(const BiomeInfo& bi);

class Tree : public Plant
{
public:
    Tree(const int ID, const int max_age, const int reproduction_age, const double health, const SimpleLayerMap* density) 
        : Plant(ID, max_age, reproduction_age, health, density) {}
    Tree(const Tree& t) 
        : Tree(t._ID, t._max_age, t._reproduction_age, t._health, t._density) {} 
	virtual bool is_dead() const;
	virtual void update(std::mt19937& gen, std::uniform_real_distribution<>& rdis, VegetationLayerMap& distribution, const int i, const int j);
};