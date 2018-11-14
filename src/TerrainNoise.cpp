#include "Noise/TerrainNoise.hpp"

double TerrainNoise::getNoise(int i, int j)
{
	double value = 0.0;
	double freq = _base_freq;
	double ampl = _amplitude;

	for(int k = 0; k < _octaves; k++)
	{
		_base_noise.SetFrequency(freq);
		_ridge_noise.SetFrequency(freq);
		freq *= 2.0;
		double tv = _base_noise.GetNoise(i, j);
		double rv = _ridge_noise.GetNoise(i, j);
		double v = (tv < rv) ? tv : 2 * rv - tv;
		double kf = 1.0 - 1.0 / (double)(0.5 + k * k);

		if(k > 0)
		{
			value += (1 - (1 - ((value + _amplitude) / 10.0)) * kf) * ampl * v;
			//sf.at(i, j) += ((sf.at(i, j) + 5) / 10.0) * ampl * v;
		}
		else
		{
			value += ampl * v;
		}

		ampl /= 2.0;
	}

	return value;
}