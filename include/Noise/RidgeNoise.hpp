#pragma once

#include "ParameterizedNoise.hpp"

class RidgeNoise : ParameterizedNoise
{
  public:
    RidgeNoise(const FastNoise& noise1, const FastNoise& noise2, const double amplitude, const double wave_length)
      :  ParameterizedNoise(noise1, amplitude, wave_length), _thresholdNoise(noise2) { }

    double get_value(int x, int y) const { _amplitude*_noise.GetNoise(x,y)/_wave_length; }
    double get_amplitude() const { return _amplitude; }
    double get_wave_length() const { return _wave_length; }
    FastNoise::NoiseType get_noise_type() const { return _noise.GetNoiseType(); }

  private:
    FastNoise _thresholdNoise;
};