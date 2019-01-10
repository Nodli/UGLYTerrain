#pragma once

#include <MultiLayerMap.hpp>

/** \addtogroup Erosion
 * @{
 */

/**
 * @brief Erodes a Multi Layer Map by a constant value
 *
 * @param layers    the Multi Layer Map to erode
 * @param k         the erosion value
 */
void erode_constant(MultiLayerMap& layers, const double k);

/**
 * @brief Erodes a Multi Layer Map using the median slope value among neighbors
 *
 * @param layers    the Multi Layer Map to erode
 * @param k         the erosion value
 */
void erode_using_median_slope(MultiLayerMap& layers, const double k);

/**
 * @brief Erodes a Multi Layer Map using the median double slope value among neighbors
 * 	  The double slope is computed as the difference of opposite values with regard to each cell
 *
 * @param layers    the Multi Layer Map to erode
 * @param k         the erosion value
 */
void erode_using_median_double_slope(MultiLayerMap& layers, const double k);

/**
 * @brief Erodes a Multi Layer Map using the mean slope value among neighbors
 *
 * @param layers    the Multi Layer Map to erode
 * @param k         the erosion value
 */
void erode_using_mean_slope(MultiLayerMap& layers, const double k);

/**
 * @brief Erodes a Multi Layer Map using the mean slope value among neighbors
 * 	  The double slope is computed as the difference of opposite values with regard to each cell
 *
 * @param layers    the Multi Layer Map to erode
 * @param k         the erosion value
 */
void erode_using_mean_double_slope(MultiLayerMap& layers, const double k);

/**
 * @brief Transports the sediments towards the neighbors in 8-connexity from a Multi Layer Map until stable
 *
 * @param layers        	the Multi Layer Map containing sediments to transport
 * @param rest_angle    	the angle over which sediments are stable
 * @param quantity_tolerance 	the quantity under which no transport occurs because the quantity is considered negligible
 */
void transport(MultiLayerMap& layers, const double rest_angle = 45, const double quantity_tolerance = 0.000000000000001);

/**
 * @brief Transports the sediments towards the neighbors in 4-connexity from a Multi Layer Map until stable
 *
 * @param layers        the Multi Layer Map containing sediments to transport
 * @param rest_angle    the angle over which sediments are stable
 * @param quantity_tolerance 	the quantity under which no transport occurs because the quantity is considered negligible
 */
void transport_4connex(MultiLayerMap& layers, const double rest_angle = 45, const double quantity_tolerance = 0.000000000000001);

/** @}*/
