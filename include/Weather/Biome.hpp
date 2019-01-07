#pragma once

#include <MultiLayerMap.hpp>

void Weather_info(const MultiLayerMap& m, const int i, const int j, double& temp, double& hum, double& water, double& lux);

SimpleLayerMap Expo(const MultiLayerMap& m);
