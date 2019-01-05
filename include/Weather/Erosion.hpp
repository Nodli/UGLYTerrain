#pragma once

#include <MultiLayerMap.hpp>

/** \addtogroup Erosion
 * @{
 */

/**
 * @brief Erodes a Multi Layer Map using a slope value
 *
 * @param layers    the Multi Layer Map to erode
 * @param k         the erosion factor
 */
void erode_slope_constant(MultiLayerMap& layers, const double k);

/**
 * @brief Function controling erosion based on a slope value
 *
 * @param slope     the slope value at the point
 * @return value of erosion
 */
float erosion_control_function(const double slope);

/**
 * @brief Erodes a Multi Layer Map using erosion_control_function
 *
 * @param layers    the Multi Layer Map to erode
 * @param k			the erosion factor
 */
void erode_slope_controled(MultiLayerMap& layers, const double k);

/**
 * @brief Erode a Multi Layer Map adding a new layer and displacing the sediments
 *
 * @param layers        the Multi Layer Map to erode
 * @param k             the erosion factor
 * @param iteration_max the maximum number of iteration for the displacement
 * @param rest_angle    the angle at which the sediments will be moving
 */
void erode_and_transport(MultiLayerMap& layers, const double k, const int iteration_max = 0, const double rest_angle = 30);

/** @}*/