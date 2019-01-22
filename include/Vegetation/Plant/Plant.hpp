#pragma once
#include <SimpleLayerMap.hpp>
#include <Weather/Biome.hpp>
#include <iostream>
class VegetationLayerMap;

class Plant
{
public:
	Plant(const int max_age, const int reproduction_age, const double health, const SimpleLayerMap* density)
		: _age(0)
		, _max_age(max_age)
		, _reproduction_age(reproduction_age)
		, _health(health)
		, _density(density) {}
	Plant(const Plant& p)
		: _age(p._age)
		, _max_age(p._max_age)
		, _reproduction_age(p._reproduction_age)
		, _health(p._health)
		, _density(p._density) {}
	virtual bool is_dead() = 0;
	virtual void update(std::mt19937& gen, std::uniform_real_distribution<>& rdis, VegetationLayerMap& distribution, const int i, const int j) = 0;
public:
	int _age;
	int _max_age;
	int _reproduction_age;
	double _health;
	const SimpleLayerMap* _density;
};