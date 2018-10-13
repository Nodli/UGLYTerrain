#include <FieldValues.hpp>

void FieldValues::add_noise(const FastNoise::NoiseType noise_type, const double amplitude, const double wave_length)
{
  FastNoise noise;
  noise.SetNoiseType(noise_type);
  _noises.push_back(ParameterizedNoise(noise, amplitude, wave_length));
}

void FieldValues::compute_harmonics(const int index, const int number)
{
  int amplitude = _noises[index].get_amplitude();
  int wave_length = _noises[index].get_wave_length();
  FastNoise::NoiseType noise_type = _noises[index].get_noise_type();

  for(int i = 0; i < number; i++)
  {
    amplitude >>= 1 ;
    wave_length >>= 1 ;
    add_noise(noise_type, amplitude, wave_length);
  }
}

double FieldValues::compute_noises_sum(const int x, const int y)
{
  double sum = 0;
  
  for(int i = 0; i < _noises.size(); i++)
  {
    sum += _noises[i].get_value(x, y);
  }

  return sum;
}