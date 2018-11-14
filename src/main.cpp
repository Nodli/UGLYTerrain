#include <iostream>
#include <MultiLayerMap.hpp>
#include <ScalarField.hpp>
#include <Noise/TerrainNoise.hpp>


int main()
{
	MultiLayerMap mlm(100, 100, { -25, -25}, {25, 25});
	mlm.new_field();
	ScalarField &sediment = mlm.new_field();
	ScalarField &bedrock = mlm.get_field(0);

	TerrainNoise t_noise(10.0, 1.0 / 200.0, 8);

	for(int j = 0; j < 100; ++j)
	{
		for(int i = 0; i < 100; i++)
		{
			bedrock.at(i, j) = t_noise.getNoise(i, j);
			sediment.at(i, j) = bedrock.get_value(i, j) + 1.0;
		}
	}

	ScalarField fusion = mlm.generate_field();
	fusion.exportAsObj("BedrockAndSediment.obj");
	mlm.erode_and_create(0.5);
	fusion = mlm.generate_field();
	fusion.exportAsObj("BedrockAndSedimentEroded.obj");

	return 0;
}
