#pragma once

#include <MultiLayerMap.hpp>

void Weather_info(const MultiLayerMap& m, const int i, const int j, double& temp, double& hum, double& water, double& lux);

ScalarField Expo(const MultiLayerMap& m);
