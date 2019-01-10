#pragma once

#include <MultiLayerMap.hpp>
#include <Weather/Hydro.hpp>

void Weather_info(const MultiLayerMap& m, const int i, const int j, double& temp, double& hum, double& water, double& lux);

/**
 * @brief Get the light exposition of a field
 *
 * @param df                The source field
 * @param nb_steps          The radius used for calculating the exposure, measured in cells of the grid
 * @param nb_sambles        The number of direction for calculating the exposure
 * @return SimpleLayerMap   A field contaning the exposition information
 */
SimpleLayerMap get_light_exposure(const DoubleField& df, const int nb_steps = 20, const int nb_samples = 20);
