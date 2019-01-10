#include <Vegetation/Vegetation.hpp>

SimpleLayerMap grass_density(const MultiLayerMap& m)
{
    SimpleLayerMap slope = SimpleLayerMap::generate_slope_map(m).normalize();
    SimpleLayerMap exposition = get_light_exposure(m).normalize();
	SimpleLayerMap water_index = get_water_indexes(m).normalize();
    SimpleLayerMap height = m.generate_field().normalize();
    SimpleLayerMap density(static_cast<Grid2d>(m));

    for(int j = 0; j < density.grid_height(); ++j)
	{
		for(int i = 0; i < density.grid_width(); i++)
		{
            double value = sqrt(water_index.value(i, j))*(1-height.value(i,j));
			density.set_value(i, j, value);
		}
	}

    return density;
}

SimpleLayerMap bush_density(const MultiLayerMap& m)
{
    SimpleLayerMap slope = SimpleLayerMap::generate_slope_map(m).normalize();
    SimpleLayerMap exposition = get_light_exposure(m).normalize();
	SimpleLayerMap water_index = get_water_indexes(m).normalize();
    SimpleLayerMap height = m.generate_field().normalize();
    SimpleLayerMap density(static_cast<Grid2d>(m));

    for(int j = 0; j < density.grid_height(); ++j)
	{
		for(int i = 0; i < density.grid_width(); i++)
		{
            double value = slope.value(i, j)*height.value(i,j)*exposition.value(i, j);
			density.set_value(i, j, value);
		}
	}

    return density;
}

SimpleLayerMap tree_density(const MultiLayerMap& m)
{
    SimpleLayerMap slope = SimpleLayerMap::generate_slope_map(m).normalize();
    SimpleLayerMap exposition = get_light_exposure(m).normalize();
	SimpleLayerMap water_index = get_water_indexes(m).normalize();
    SimpleLayerMap height = m.generate_field().normalize();
    SimpleLayerMap density(static_cast<Grid2d>(m));

    for(int j = 0; j < density.grid_height(); ++j)
	{
		for(int i = 0; i < density.grid_width(); i++)
		{
            double value = water_index.value(i, j)*exposition.value(i, j);
			density.set_value(i, j, value);
		}
	}

    return density;
}

void generate_distribution(const MultiLayerMap& m)
{
    int maxVal = 255;
    SimpleLayerMap distrib(static_cast<Grid2d>(m));
    distrib.set_all(0.0);
    MultiLayerMap distribs(static_cast<Grid2d>(m));
    distribs.add_field(grass_density(m));
    distribs.add_field(bush_density(m));
    distribs.add_field(tree_density(m));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis_width(0, m.grid_width() - 1);
	std::uniform_int_distribution<> dis_height(0, m.grid_width() - 1);
    std::uniform_real_distribution<> rdis(0, 1);

    bool nope = true;
    // for(int i =0; i < 1500; ++i)
    // {
    //     nope = true;
    //     do{
	// 	    int x = dis_width(gen);
	// 	    int y = dis_height(gen);
    //         if(rdis(gen) < distribs.get_field(0).value(x, y))
    //         {
    //             distrib.set_value(x, y, 1.0);
    //             nope = false;
    //         }
    //     }while(nope);
    // }

    // for(int i =0; i < 400; ++i)
    // {
	// 	nope = true;
    //     do{
	// 	    int x = dis_width(gen);
	// 	    int y = dis_height(gen);
    //         if(rdis(gen) < distribs.get_field(1).value(x, y))
    //         {
    //             distrib.set_value(x, y, 2.0);
    //             nope = false;
    //         }
    //     }while(nope);
    // }

    for(int i =0; i < 400; ++i)
    {
		nope = true;
        do{
		    int x = dis_width(gen);
		    int y = dis_height(gen);
            if(rdis(gen) < distribs.get_field(2).value(x, y))
            {
                distrib.set_value(x, y, 3.0);
                nope = false;
            }
        }while(nope);
    }

    distrib.export_as_pgm("TEST.pgm", true);

    std::string filename = "distribT.ppm";
	std::ofstream output(filename, std::ofstream::out);
	output << "P3" << std::endl;
	output << distrib.grid_width() << " " << distrib.grid_height() << std::endl;
	output << maxVal << std::endl;

	for(int j = distrib.grid_height() - 1; j >= 0; --j)
	{
		for(int i = 0; i < distrib.grid_width(); ++i)
		{
            if(distrib.value(i, j) == 0)
            {
                output << 0 << " " << 0 << " " << 0 << " ";
            }
            else if(distrib.value(i, j) == 1)
            {
                output << 30 << " " << 255 << " " << 10 << " ";
            }
            else if(distrib.value(i, j) == 2)
            {
                output << 100 << " " << 125 << " " << 20 << " ";
            }
            else if(distrib.value(i, j) == 3)
            {
                output << 255 << " " << 100 << " " << 100 << " ";
            }    
        }

		output << std::endl;
	}
}