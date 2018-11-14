#include <iostream>
#include <MultiLayerMap.hpp>
#include <ScalarField.hpp>
#include <Noise/TerrainNoise.hpp>

int main()
{
	MultiLayerMap mlm(100, 100, { -25, -25}, {25, 25});
	ScalarField &sf = mlm.new_field();
	TerrainNoise t_noise(10.0, 1.0 / 200.0, 8);

	for(int j = 0; j < 100; ++j)
	{
		for(int i = 0; i < 100; i++)
		{
			sf.at(i, j) = t_noise.getNoise(i, j);
		}
	}

	sf.exportAsObj("Terrain.obj");
	mlm.erode(0.5);
	sf.exportAsObj("TerrainErode.obj");
	return 0;
}

