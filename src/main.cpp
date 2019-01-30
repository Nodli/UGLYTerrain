#include <iostream>
#include <random>
#include <string>
#include <MultiLayerMap.hpp>
#include <SimpleLayerMap.hpp>
#include <Noise/TerrainNoise.hpp>
#include <Weather/Erosion.hpp>
#include <Weather/Hydro.hpp>
#include <Weather/Biome.hpp>

void test_thermal_erosion_transport_stair(unsigned int iterations = 101, unsigned int save_period = 10){
	const int size = 10;

	// setup a test MultiLayerMap with a stair terrain
	MultiLayerMap mlm(size, size);
	SimpleLayerMap& height = mlm.new_layer();

	//height = stair_layer(size, size, 1.);
	height = double_stair_layer(size, size, 1.);

	mlm.get_field(0).export_as_obj("InitialTerrain.obj");

	for(int istep = 0; istep != iterations; ++istep){
		std::string folder_name = "ErosionTransport" + std::to_string(istep);
		std::string sys_cmd = "mkdir " + folder_name;
		if(istep % save_period == 0){
			system(sys_cmd.c_str());
		}

		std::cout << "===== STARTING ITERATION " << istep << " =====" << std::endl;
		// testing erosion without transport
		erode_constant(mlm, 0.1);
		//erode_using_median_slope(mlm, 0.1);
		//erode_using_mean_slope(mlm, 0.1);
		//erode_using_median_double_slope(mlm, 0.1);
		//erode_using_mean_double_slope(mlm, 0.1);

		if(istep % save_period == 0){
			mlm.get_field(0).export_as_obj("./" + folder_name + "/ThermalErosionTerrainBedrock.obj");
			mlm.get_field(1).export_as_obj("./" + folder_name + "/ThermalErosionTerrainSediments.obj");
			mlm.generate_field().export_as_obj("./" + folder_name + "/ThermalErosionTerrain.obj");
		}

		// transport on the previously eroded terrain
		transport(mlm, 25.);
		//transport_4connex(mlm, 25.);
		if(istep % save_period == 0){
			mlm.get_field(0).export_as_obj("./" + folder_name + "/ThermalTransportTerrainBedrock.obj");
			mlm.get_field(1).export_as_obj("./" + folder_name + "/ThermalTransportTerrainSediments.obj");
			mlm.generate_field().export_as_obj("./" + folder_name + "/ThermalTransportTerrain.obj");
		}


		MultiLayerMap normalized_mlm = normalized(mlm);
		normalized_mlm.generate_field().export_as_obj("normalized.obj");
	}
}

int main()
{
	//test_thermal_erosion_transport_stair();

	std::random_device rd;
	std::mt19937 gen(rd());

	int size = 500;
	MultiLayerMap mlm(size, size, { -5, -5}, {5, 5});
	SimpleLayerMap &sf = mlm.new_layer();
	TerrainNoise t_noise(2.2, 0.004, 8);

	for(int j = 0; j < size; ++j)
	{
		for(int i = 0; i < size; i++)
		{
			sf.at(i, j) = t_noise.get_noise3(i, j);
		}
	}

	sf.export_as_obj("InitialTerrain.obj");
	sf.export_as_pgm("InitialTerrain.pgm", true);
	//SimpleLayerMap::generate_slope_map(sf).export_as_pgm("Slope.pgm", true);

	/*
	// determining layering
	std::vector<double> material_layers_top = {0.09, 0.15,
						0.29, 0.31,
						0.63, 0.71,
						0.79, 0.83};
	std::vector<double> material_resistances = {0.01, 0.00001,
						0.01, 0.00002,
						0.01, 0.00001,
						0.01, 0.00003,
						0.01};
	// --- ///
	std::vector<double> material_layers_top = {0.09, 0.11,
						0.19, 0.21,
						0.29, 0.31,
						0.39, 0.41,
						0.49, 0.51,
						0.59, 0.61,
						0.69, 0.71,
						0.79, 0.81,
						0.89, 0.91};
	std::vector<double> material_resistances = {0.01, 0.00001,
						0.01, 0.00001,
						0.01, 0.00001,
						0.01, 0.00001,
						0.01};
	std::vector<double> material_resistances = {0.01, 0.00001,
						0.01, 0.00001,
						0.01, 0.00001,
						0.01, 0.00001,
						0.01, 0.00001,
						0.01, 0.00001,
						0.01};
	*/

	std::vector<double> material_layers_top; 
	/*{0.09, 0.11,
						0.29, 0.31,
						0.39, 0.41,
						0.79, 0.81,
						0.84, 0.86};*/
	std::vector<double> material_resistances;
	/* = {0.01, 0.00001,
						0.01, 0.00001,
						0.01, 0.00001,
						0.01, 0.00001,
						0.01, 0.00001,
						0.01};*/
	int nb_layers = 100;
	double delta = 1.0/nb_layers;
	std::uniform_real_distribution layer_disp(0.0, delta);
	std::uniform_real_distribution layer_solid(0.0, 1.0);
	for(int i = 0; i < nb_layers; ++i)
	{
		material_layers_top.push_back(i*delta+layer_disp(gen));
		if(i%2 == 0){
			material_resistances.push_back(0.005 + 0.01*layer_solid(gen));
		}else{
			material_resistances.push_back(0.00001 + 0.001*layer_solid(gen));
		}
	}

	// rescaling layers to the whole height range
	const double bottom_height = sf.get_min();
	const double height_range = sf.get_max() - bottom_height;
	for(int ilayer = 0; ilayer != material_layers_top.size(); ++ilayer){
		material_layers_top[ilayer] = bottom_height + material_layers_top[ilayer] * height_range;
	}

	const int erosion_transport_iterations = 101;
	const int save_period = 5;
	for(int istep = 0; istep != erosion_transport_iterations; ++istep){
		std::cout << "iteration " << istep + 1 << "..." << std::endl;
		std::string folder_name = "ErosionTransport" + std::to_string(istep);
		std::string sys_cmd = "mkdir " + folder_name;
		if(istep % save_period == 0){
			system(sys_cmd.c_str());
		}

		// Thermal erosion
		//erode_constant(mlm, 0.01);
		//erode_using_mean_double_slope(mlm, 0.01);
		//erode_using_exposure(mlm, 0.01);
		erode_layered_materials_using_exposure(mlm, material_layers_top, material_resistances, 10);
		if(istep % save_period == 0){
			mlm.get_field(0).export_as_obj("./" + folder_name + "/ThermalErosionTerrainBedrock.obj");
			mlm.get_field(1).export_as_obj("./" + folder_name + "/ThermalErosionTerrainSediments.obj");
			mlm.generate_field().export_as_obj("./" + folder_name + "/ThermalErosionTerrain.obj");
		}

		// Thermal transport
		transport(mlm, 30);
		//transport_4connex(mlm, 30);
		if(istep % save_period == 0){
			mlm.get_field(0).export_as_obj("./" + folder_name + "/ThermalTransportTerrainBedrock.obj");
			mlm.get_field(1).export_as_obj("./" + folder_name + "/ThermalTransportTerrainSediments.obj");
			mlm.generate_field().export_as_obj("./" + folder_name + "/ThermalTransportTerrain.obj");
		}

		// Water drop erosion
		/*
		water_drop_transport(mlm, gen, 1000, 0.01, 0.1);
		if(istep % save_period == 0){
			mlm.get_field(0).export_as_obj("./" + folder_name + "WaterDropTerrainBedrock.obj");
			mlm.get_field(1).export_as_obj("./" + folder_name + "WaterDropTerrainSediments.obj");
			mlm.generate_field().export_as_obj("./" + folder_name + "WaterDropTerrain.obj");
		}
		*/

		std::cout << "done" << std::endl;
	}

/*
	// Hydraulic erosion, area visualization
	mlm.new_layer();

	SimpleLayerMap area_distributed = get_area(mlm.generate_field());
	SimpleLayerMap area_steepest 		= get_area(mlm.generate_field(), false);
	area_distributed.normalize();
	area_steepest.normalize();
	area_distributed.export_as_pgm("DistributedHydraulicArea.pgm", true);
	area_steepest.export_as_pgm("OneWayHydraulicArea.pgm", true);

	int filter_size = 3;
	SimpleLayerMap filter(filter_size, filter_size);
	filter.set_all(1.0/(double) (filter_size * filter_size));

	std::vector<std::pair<double, Eigen::Vector2i>> convoluted_values = area_steepest.full_convolution(filter);
	area_steepest.import_list(convoluted_values);
	area_steepest.export_as_pgm("OneWayConvolutedHydraulicArea.pgm", true);

	convoluted_values = area_distributed.full_convolution(filter);
	area_distributed.import_list(convoluted_values);
	area_distributed.export_as_pgm("DistributedConvolutedHydraulicArea.pgm", true);

	// Hydraulic erosion, terrain visualization
	MultiLayerMap mlmBis(mlm);
	MultiLayerMap mlmTer(mlm);
	MultiLayerMap mlmQua(mlm);
	MultiLayerMap mlmQui(mlm);
/*
	// distributed erode
	erode_from_area(mlm, area_distributed, 0.2);
	mlm.generate_field().export_as_pgm("TerrainDistributedHydroErode.pgm", true);
	mlm.generate_field().export_as_obj("TerrainDistributedHydroErode.obj");

	// distributed erode and transport
	erode_from_area(mlmBis, area_distributed, 0.2, true, 0.05);
	mlmBis.generate_field().export_as_pgm("TerrainDistributedHydroErodeAndTransport.pgm", true);
	mlmBis.generate_field().export_as_obj("TerrainDistributedHydroErodeAndTransport.obj");

	// one way erode
	erode_from_area(mlmTer, area_steepest, 0.2);
	mlmTer.generate_field().export_as_pgm("TerrainOneWayHydroErode.pgm", true);
	mlmTer.generate_field().export_as_obj("TerrainOneWayHydroErode.obj");

	// one way erode and transport
	erode_from_area(mlmQua, area_steepest, 0.2, true, 0.05);
	mlmQua.generate_field().export_as_pgm("TerrainOneWayHydroErodeAndTransport.pgm", true);
	mlmQua.generate_field().export_as_obj("TerrainOneWayHydroErodeAndTransport.obj");
	// water drop
	filter.set_all(0.05);
	filter.at(1, 1) = 0.6;
	erode_from_droplets(mlmQui, gen, filter, 100000, 0.01, 0.01, 0.1);
	mlmQui.generate_field().export_as_pgm("TerrainWaterDropHydroErodeAndTransport.pgm", true);
	mlmQui.generate_field().export_as_obj("TerrainWaterDropHydroErodeAndTransport.obj");
*/
	return 0;
}

