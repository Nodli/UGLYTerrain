#pragma once

#include <MultiLayerMap.hpp>

/** \addtogroup Hydro
 * @{
 */
/**
 * @brief Compute Hydraulic area from slope for a Multi Layer Map
 *
 * @param layers          the Multi Layer Map to erode
 * @return ScalarField    the computed area
 */
ScalarField get_area(MultiLayerMap& layers);

/** @}*/