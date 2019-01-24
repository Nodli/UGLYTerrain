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
 * @brief Erodes a Multi Layer Map using the exposure of each cell 
 *
 * @param layers    the Multi Layer Map to erode
 * @param k         the erosion value
 */
void erode_using_exposure(MultiLayerMap& layers, const double k);

/**
 * @brief Erodes a Multi Layer Map using the exposure in a multi-material context 
 *
 * @param layers    			the Multi Layer Map to erode
 * @param layers_top_heights		list of the heights of the top of the layers, in increasing order
 * @param layers_erosion_values		list of the erosion values for the layers, in the same order as layers_top_heights
 *					with one more value for the topmost 'infinite' layer
 */
void erode_layered_materials_using_exposure(MultiLayerMap& layers,
					const std::vector<double>& layers_top_heights,
					const std::vector<double>& layers_erosion_values,
					const double layers_angle = 0.);

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
