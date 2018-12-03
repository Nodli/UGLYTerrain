#pragma once

#include <MultiLayerMap.hpp>

/** \addtogroup Hydro
 * @{
 */
/**
 * @brief Compute Hydraulic area from an heightmap
 *
 * @param heightMap       the source for the computation
 * @return ScalarField    the computed area
 */
ScalarField get_area(const ScalarField& heightmap);

void erode_from_area(MultiLayerMap& layers, double k);
/** @}*/