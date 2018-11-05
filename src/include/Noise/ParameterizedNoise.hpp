#pragma once

#include "FastNoise.hpp"

class ParameterizedNoise
{
  public:
    ParameterizedNoise(const FastNoise& noise, const double amplitude, const double wave_length)
      : _noise(noise), _amplitude(amplitude), _wave_length(wave_length) {}

    double get_value(int x, int y) const { _amplitude*_noise.GetNoise(x,y)/_wave_length; }
    double get_amplitude() const { return _amplitude; }
    double get_wave_length() const { return _wave_length; }
    FastNoise::NoiseType get_noise_type() const { return _noise.GetNoiseType(); }

  private:
    FastNoise _noise;
    double _amplitude;
    double _wave_length;
};