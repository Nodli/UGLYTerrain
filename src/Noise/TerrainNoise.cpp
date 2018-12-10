#include "Noise/TerrainNoise.hpp"
FastNoise noise;
//noise.SetNoiseType(FastNoise::Perlin);

double TerrainNoise::get_noise(int i, int j)
{
	double value = 0.0;
	double freq = _base_freq;
	double ampl = _amplitude;
	noise.SetFrequency(_base_freq/2.0);
	double noi = (0.5 + 0.5*noise.GetNoise(i, j));
	for(int k = 0; k < _octaves; k++)
	{
		_base_noise.SetFrequency(freq);
		_ridge_noise.SetFrequency(freq);
		freq *= 2.0;
		double tv = _base_noise.GetNoise(i + k * 100, j + k * 100);
		double rv = _ridge_noise.GetNoise(i + k * 100, j + k * 100);
		double v = (tv < rv) ? tv : 2 * rv - tv;
		double kf = 1.0 - 1.0 / (double)(0.5 + k * k);

		if(k > 0)
		{
			value += (1 - (1 - ((value + _amplitude) / (2.0*_amplitude))) * kf) * ampl * v;
			//sf.at(i, j) += ((sf.at(i, j) + 5) / 10.0) * ampl * v;
		}
		else
		{
			value += ampl * v;
		}

		ampl /= 2.0;
	}

	return (value+2*_amplitude);//*noi;
}