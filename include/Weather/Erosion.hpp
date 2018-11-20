#pragma once

#include <MultiLayerMap.hpp>

/** \addtogroup Erosion
 * @{
 */
/**
 * @brief Basicaly erode a Multi Layer Map
 *
 * @param layers    the Multi Layer Map to erode
 * @param k         the erosion factor
 */
void erode(MultiLayerMap& layers, const double k);

/**
 * @brief Erode a Multi Layer Map adding a new layer and displacing the sediments
 *
 * @param layers        the Multi Layer Map to erode
 * @param k             the erosion factor
 * @param iteration_max the maximum number of iteration for the displacement
 * @param rest_angle    the angle at which the sediments will be moving
 */
void erode_and_transport(MultiLayerMap& layers, const double k, const int iteration_max = 0, const double rest_angle = 90);

/** @}*/