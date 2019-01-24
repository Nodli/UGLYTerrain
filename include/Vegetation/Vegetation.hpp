#pragma once

#include <MultiLayerMap.hpp>
#include <Weather/Biome.hpp>

/**
 * @brief Test for generation density function
 * 
 */
SimpleLayerMap grass_density(const MultiLayerMap& m);

SimpleLayerMap bush_density(const MultiLayerMap& m);

SimpleLayerMap tree_density(const MultiLayerMap& m);

void generate_distribution(const MultiLayerMap& m);