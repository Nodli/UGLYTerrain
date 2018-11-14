#include <Noise/MultiNoise.hpp>

void MultiNoise::add_noise(const FastNoise::NoiseType noise_type, const double amplitude, const double wave_length)
{
	FastNoise noise;
	noise.SetNoiseType(noise_type);
	_noises.push_back(ParameterizedNoise(noise, amplitude, wave_length));
}

void MultiNoise::compute_octaves(const int index, const int number)
{
	if(index < _noises.size())
	{
		ParameterizedNoise& noise = _noises[index];
		double amplitude = noise.get_amplitude();
		double wave_length = noise.get_wave_length();
		FastNoise::NoiseType noise_type = noise.get_noise_type();

		for(int i = 0; i < number; i++)
		{
			amplitude /= 2 ;
			wave_length /= 2 ;
			add_noise(noise_type, amplitude, wave_length);
		}
	}
}

double MultiNoise::compute_noises_sum(const int x, const int y)
{
	double sum = 0;

	for(int i = 0; i < _noises.size(); i++)
	{
		sum += _noises[i].get_value(x, y);
	}

	return sum;
}