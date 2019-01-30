#pragma once

#include <MultiLayerMap.hpp>
#include <Weather/Biome.hpp>
#include <Vegetation/VegetationLayerMap.hpp>
#include <Vegetation/MountainFlore.hpp>

/**
 * @brief generate a basic distribution of species
 * 
 * @param mlm       the input terrain for the simulation
 */
void generate_distribution(const MultiLayerMap& m);

/**
 * @brief simulate a basic ecosystem on a terrain
 * 
 * @param mlm       the input terrain for the simulation
 */
void simulate(const MultiLayerMap& mlm);