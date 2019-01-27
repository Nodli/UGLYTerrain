#pragma once

#include <random>
#include <MultiLayerMap.hpp>

/** \addtogroup Hydro
 * @{
 */

/**
 * @brief Compute Hydraulic area from an heightmap
 *
 * @param heightMap         the source for the computation
 * @param distribute        wether or not the area of a point should be distributed on the neighbors
 * @return SimpleLayerMap   the computed area
 */
SimpleLayerMap get_area(const DoubleField& heightmap, bool distribute = true);

/**
 * @brief Get the water indexes of an heightmap
 * 
 * @param heightmap         the map to get the index from
 * @return SimpleLayerMap   the map of the computed water index
 */
SimpleLayerMap get_water_indexes(const DoubleField& heightmap);

/**
 * @brief Erode and transport from Hydraulic area
 *
 * @param layers        the source for heightmap computation
 * @param area          the Hydraulic area used for erosion and transportation
 * @param k             intensity of erosion
 * @param kd            deposition weight, high value leads to more materials on high area values
 * @param transport     wether or not transportation is performed after erosion
 */
void erode_from_area(MultiLayerMap& layers, const SimpleLayerMap& area, double k, double kd = 0.1, bool transport = true);

/**
 * @brief Erode and transport using droplets
 *
 * @param layers          the source for heightmap computation
 * @param gen             the random numbers generator
 * @param brush           distribute droplet effect among neighbors according to the given pattern
 * @param n               number of droplets
 * @param water_loss      water quantity loss per iteration
 * @param k               intensity of erosion per droplet
 * @param kd              deposition weight, high value leads to more flat plains
 */
void erode_from_droplets(MultiLayerMap& layers, std::mt19937& gen, const SimpleLayerMap& brush, int n, double water_loss, double k, double kd = 0.5);
/** @}*/
