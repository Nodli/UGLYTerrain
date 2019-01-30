#pragma once

#include <MultiLayerMap.hpp>
#include <Weather/Hydro.hpp>

/** \addtogroup Biomes
 * @{
 */

struct BiomeInfo
{
    BiomeInfo(const MultiLayerMap& m);

    SimpleLayerMap slope;
    SimpleLayerMap exposure;
    SimpleLayerMap water_index;
    SimpleLayerMap height;
    SimpleLayerMap sediments;
};

/**
 * @brief Get the light exposition of a field
 *
 * @param df                The source field
 * @param nb_steps          The radius used for calculating the exposure, measured in cells of the grid
 * @param nb_sambles        The number of direction for calculating the exposure
 * @return SimpleLayerMap   A field contaning the exposition information
 */
SimpleLayerMap get_light_exposure(const DoubleField& df, const int nb_steps = 20, const int nb_samples = 10);

/**
 * @brief saves a texture of the multilayer map
 * 
 * @param mlm 					the source multilayermap
 */
void save_colorized(const MultiLayerMap& mlm);

/** @}*/