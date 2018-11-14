#pragma once

#include <vector>
#include <FastNoise/FastNoise.hpp>
#include <Noise/ParameterizedNoise.hpp>

class FieldValues
{
public:
	void add_noise(const FastNoise::NoiseType noise_type, const double amplitude, const double wave_length);
	void compute_octaves(const int index, const int number);
	double compute_noises_sum(const int x, const int y);

	int get_noises_number()
	{
		return _noises.size();
	}
	ParameterizedNoise& get_noise(unsigned int index)
	{
		return _noises[index];
	}

private:
	std::vector<ParameterizedNoise> _noises;
};