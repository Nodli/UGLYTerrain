#pragma once

#include <MultiLayerMap.hpp>

/** \addtogroup Hydro
 * @{
 */

/**
 * @brief Compute Hydraulic area from an heightmap
 *
 * @param heightMap       the source for the computation
 * @param distribute      weither or not the area of a point should be distributed on the neighbors
 * @return ScalarField    the computed area
 */
ScalarField get_area(const ScalarField& heightmap, bool distribute = true);

/**
 * @brief Erode from Hydraulic area
 *
 * @param layers        the source for heightmap and hydraulic area computation
 * @param k             intensity of erosion
 * @param distribute    weither or not the area of a point should be distributed on the neighbors
 */
void erode_from_area(MultiLayerMap& layers, double k, bool distribute = true);
/** @}*/