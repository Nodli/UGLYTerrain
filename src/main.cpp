#include <iostream>
#include <MultiLayerMap.hpp>
#include <ScalarField.hpp>
#include <Noise/TerrainNoise.hpp>
#include <Weather/Erosion.hpp>
#include <Weather/Hydro.hpp>

int main()
{
	int size = 100 ;
	MultiLayerMap mlm(size, size, { -5, -5}, {5, 5});
	ScalarField &sf = mlm.new_field();
	TerrainNoise t_noise(2.5, 1.0 / 100.0, 8);

	for(int j = 0; j < size; ++j)
	{
		for(int i = 0; i < size; i++)
		{
			sf.at(i, j) = t_noise.get_noise(i, j);
		}
	}

	sf.export_as_obj("Terrain.obj");
	sf.export_as_pgm("Terrain.pgm", true);
	sf.get_slope_map().export_as_pgm("Slope.pgm", true);
	/*erode_and_transport(mlm, 0.1, 1);
	mlm.get_field(0).export_as_pgm("TerrainBedrock.pgm", true);
	mlm.get_field(0).export_as_obj("TerrainBedrock.obj");
	mlm.get_field(1).export_as_obj("TerrainSediments.obj");
	mlm.generate_field().export_as_obj("ErodedTerrain.obj");*/
	ScalarField area = get_area(mlm.generate_field());
	area.export_as_pgm("HydraulicArea.pgm", true);
	erode_from_area(mlm, 0.1);
	mlm.get_field(0).export_as_pgm("TerrainHydroErode.pgm", true);
	mlm.get_field(0).export_as_obj("TerrainHydroErode.obj");

	return 0;
}

