#pragma once

#include <MultiLayerMap.hpp>
#include <Weather/Biome.hpp>
#include <Vegetation/VegetationLayerMap.hpp>
#include <Vegetation/MountainFlore.hpp>

void generate_distribution(const MultiLayerMap& m);

void simulate(const MultiLayerMap& mlm);

//void simulate(ensemble d'individus)
/*
pour chaque individu :
    age += dt
    est-ce qu'il meurt ?
    est-ce qu'il se reproduit ?
    est-ce qu'il se développe (fonction environnement)
    règles de dominance
*/