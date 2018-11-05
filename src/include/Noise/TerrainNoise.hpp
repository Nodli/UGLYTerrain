#include <FastNoise.hpp>

class TerrainNoise
{
public:
	TerrainNoise(double ampl, double freq, int oct, int s1 = 0, int s2 = 4) :
		_amplitude(ampl), _base_freq(freq), _octaves(oct)
	{
		_base_noise.SetSeed(s1);
		_base_noise.SetNoiseType(FastNoise::Perlin);
		_ridge_noise.SetSeed(s2);
		_ridge_noise.SetNoiseType(FastNoise::Perlin);
	}

	double getNoise(int i, int j);

private:
	FastNoise _base_noise;
	FastNoise _ridge_noise;
	double _amplitude;
	double _base_freq;
	int _octaves;
};