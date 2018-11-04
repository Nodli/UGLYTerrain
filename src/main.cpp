#include <iostream>
#include <ScalarField.hpp>
#include <FastNoise.hpp>

int main()
{
	ScalarField sf(500, 500, { -25, -25}, {25, 25});
	FastNoise terrainNoise;
	FastNoise ridgeNoise;
	double freq = 1.0 / 200.0;
	double ampl = 5.0;
	terrainNoise.SetNoiseType(FastNoise::Perlin);
	ridgeNoise.SetNoiseType(FastNoise::Perlin);
	ridgeNoise.SetSeed(4);

	for(int k = 0; k < 10; k++)
	{
		terrainNoise.SetFrequency(freq);
		ridgeNoise.SetFrequency(freq);
		freq *= 2.0;

		for(int j = 0; j < 500; ++j)
		{
			for(int i = 0; i < 500; i++)
			{
				double tv = terrainNoise.GetNoise(i, j);
				double rv = ridgeNoise.GetNoise(i, j);
				double v = (tv < rv) ? tv : 2 * rv - tv;
				double kf = 1.0 - 1.0 / (double)(0.5 + k * k); //sqrt(sqrt((double)k / 10.0));

				if(k > 0)
				{
					sf.at(i, j) += (1 - (1 - ((sf.at(i, j) + 5) / 10.0)) * kf) * ampl * v;
					//sf.at(i, j) += ((sf.at(i, j) + 5) / 10.0) * ampl * v;
				}
				else
				{
					sf.at(i, j) += ampl * v;
				}
			}
		}

		ampl /= 2.0;
	}

	sf.exportAsObj("Terrain.obj");
	return 0;
}
