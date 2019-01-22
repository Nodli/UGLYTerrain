#pragma once

#include <MultiLayerMap.hpp>
#include <Weather/Biome.hpp>
#include <Vegetation/VegetationLayerMap.hpp>
#include <Vegetation/Plant/Grass.hpp>
/**
 * @brief Test for generation density function
 * 
*/
// SimpleLayerMap grass_density(const MultiLayerMap& m);

SimpleLayerMap bush_density(const MultiLayerMap& m);

SimpleLayerMap tree_density(const MultiLayerMap& m);

void generate_distribution(const MultiLayerMap& m);

void simulate(const MultiLayerMap& mlm);

//void simulate(ensemble d'individus)
/*
pour chaque individu :
    age += dt
    est-ce qu'il meurt ?
    est-ce qu'il se reproduit ?
    est-ce qu'il se développe (fonction environnement)
    règles de dominance
*/