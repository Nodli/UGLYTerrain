#include <iostream>
#include <ScalarField.hpp>
#include <Noise/TerrainNoise.hpp>


int main()
{
	ScalarField sf(500, 500, { -25, -25}, {25, 25});
	TerrainNoise t_noise(5.0, 1.0 / 200.0, 8);

	for(int j = 0; j < 500; ++j)
	{
		for(int i = 0; i < 500; i++)
		{
			sf.at(i, j) = t_noise.getNoise(i, j);
		}
	}

	sf.exportAsObj("Terrain.obj");
	return 0;
}
