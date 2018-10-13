#pragma once

#include <vector>
#include "FastNoise.hpp"
#include "ParameterizedNoise.hpp"

class FieldValues
{
  public:
    FieldValues();
    void add_noise(const FastNoise::NoiseType noise_type, const double amplitude, const double wave_length);
    void compute_harmonics(const int index, const int number);
    double compute_noises_sum(const int x, const int y);
    
  private:
    std::vector<ParameterizedNoise> _noises ;
};