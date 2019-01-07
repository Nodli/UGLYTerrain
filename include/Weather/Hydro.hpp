#pragma once

#include <random>
#include <MultiLayerMap.hpp>

/** \addtogroup Hydro
 * @{
 */

/**
 * @brief Compute Hydraulic area from an heightmap
 *
 * @param heightMap       the source for the computation
 * @param distribute      weither or not the area of a point should be distributed on the neighbors
 * @return SimpleLayerMap    the computed area
 */
SimpleLayerMap get_area(const DoubleField& heightmap, bool distribute = true);

/**
 * @brief Erode from Hydraulic area
 *
 * @param layers        the source for heightmap and hydraulic area computation
 * @param k             intensity of erosion
 * @param distribute    weither or not the area of a point should be distributed on the neighbors
 */
void erode_from_area(MultiLayerMap& layers, double k, bool distribute = true);

/**
 * @brief Erode and transport from hydraulic area, drop by drop
 *
 * @param layers          the source for heightmap and hydraulic area computation
 * @param n               number of water drops
 * @param water_per_drop  initial quantity of water in a drop
 * @param water_loss      water quantity loss per iteration
 * @param initial_speed   initial speed of drops
 */
void water_drop_transport(MultiLayerMap& layers, std::mt19937& gen, int n, double water_loss, double k);
/** @}*/