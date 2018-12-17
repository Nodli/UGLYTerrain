#include <iostream>
#include <random>
#include <MultiLayerMap.hpp>
#include <ScalarField.hpp>
#include <Noise/TerrainNoise.hpp>
#include <Weather/Erosion.hpp>
#include <Weather/Hydro.hpp>

int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());

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

	// Hydraulic erosion, area visualization
	ScalarField area = get_area(mlm.generate_field());
	area.export_as_pgm("DistributedHydraulicArea.pgm", true);
	area = get_area(mlm.generate_field(), false);
	area.export_as_pgm("OneWayHydraulicArea.pgm", true);

	// Hydraulic erosion, terrain visualization
	ScalarField sediments = mlm.new_field();
	sediments.set_all(0.0);

	MultiLayerMap mlmBis(mlm);
	MultiLayerMap mlmTer(mlm);
	// distributed
	erode_from_area(mlm, 0.2);
	mlm.get_field(0).export_as_pgm("TerrainDistributedHydroErode.pgm", true);
	mlm.get_field(0).export_as_obj("TerrainDistributedHydroErode.obj");
	mlm.generate_field().export_as_obj("TerrainDistributedHydroErodeAndTransport.obj");
	// one way
	erode_from_area(mlmBis, 0.2, false);
	mlmBis.get_field(0).export_as_pgm("TerrainOneWayHydroErode.pgm", true);
	mlmBis.get_field(0).export_as_obj("TerrainOneWayHydroErode.obj");
	mlmBis.generate_field().export_as_obj("TerrainOneWayHydroErodeAndTransport.obj");
	// water drop
	water_drop_transport(mlmTer, gen, 10, 1.0, 0.1, 1.0);
	mlmTer.get_field(0).export_as_pgm("TerrainWaterDropHydroErodeAndTransport.pgm", true);
	mlmTer.get_field(0).export_as_obj("TerrainWaterDropHydroErodeAndTransport.obj");

	return 0;
}

