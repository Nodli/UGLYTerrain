#pragma once

#include <MultiLayerMap.hpp>

/** \addtogroup Hydro
 * @{
 */
/**
 * @brief Compute Hydraulic erosion for a Multi Layer Map
 *
 * @param layers    the Multi Layer Map to erode
 */
ScalarField get_area(MultiLayerMap& layers);

/** @}*/