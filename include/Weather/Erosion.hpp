#pragma once

#include <MultiLayerMap.hpp>

/**
 * @brief Basicaly erode a Multi Layer Map
 *
 * @param layers    the Multi Layer Map to erode
 * @param k         the erosion factor
 */
void erode(MultiLayerMap& layers, double k);

/**
 * @brief Erode a Multi Layer Map adding a new layer
 *
 * @param layers    the Multi Layer Map to erode
 * @param k         the erosion factor
 */
void erode_and_create(MultiLayerMap& layers, double k);