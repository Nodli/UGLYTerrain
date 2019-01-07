#pragma once

#include <MultiLayerMap.hpp>
#include <Weather/Hydro.hpp>

void Weather_info(const MultiLayerMap& m, const int i, const int j, double& temp, double& hum, double& water, double& lux);

/**
 * @brief Get the light exposition of a MultiLayerMap
 * 
 * @param m                 The source MultiLayerMap
 * @return SimpleLayerMap   A field contaning the exposition information
 */
SimpleLayerMap get_light_exposition(const MultiLayerMap& m);

/**
 * @brief Test for generation density function
 * 
 */
SimpleLayerMap grass_density(const MultiLayerMap& m);

SimpleLayerMap bush_density(const MultiLayerMap& m);

SimpleLayerMap tree_density(const MultiLayerMap& m);

void generate_distribution(const MultiLayerMap& m);