#include <iostream>
#include <MultiLayerMap.hpp>
#include <ScalarField.hpp>
#include <Noise/TerrainNoise.hpp>

int main()
{
	MultiLayerMap mlm(100, 100, { -5, -5}, {5, 5});
	ScalarField &sf = mlm.new_field();
	TerrainNoise t_noise(2.5, 1.0 / 100.0, 8);

	for(int j = 0; j < 100; ++j)
	{
		for(int i = 0; i < 100; i++)
		{
			sf.at(i, j) = t_noise.get_noise(i, j);
		}
	}

	sf.exportAsObj("Terrain.obj");
	sf.get_slope_map().exportAsPgm("Terrain.pgm", true);
	mlm.erode(0.5);
	sf.exportAsObj("TerrainErode.obj");
	return 0;
}

