#include <Weather/Biome.hpp>

void Weather_info(const MultiLayerMap& m, const int i, const int j, double& temp, double& hum, double& water, double& lux)
{
    temp = 100./m.get_sum(i, j);
    hum = m.get_field(1).value(i, j);
    int nb_dir = 16;
    int nb_step = 3;
    double val = m.get_sum(i, j);
    double total = nb_dir*3.1415/2.0;
    double sum_exp = 0;
    for(int d = 0; d < nb_dir; d++)
    {
        double angle = (2.*3.1415)*d/nb_dir;
        Eigen::Vector2d delta_pos = {cos(angle), sin(angle)};
        double covA = 0;
        double h = 0;
        for(int s = 0; s < nb_step; ++s)
        {
            double v = m.get_sum(i+s*delta_pos(0), j+s*delta_pos(1)) - val;
            if (v > 0)
            {
                double tmpCov = atan(v/delta_pos.norm());
                if(tmpCov > covA)
                {
                    covA = tmpCov;
                }
            }
        }
        sum_exp += (3.1415/2.0)-covA;
    }
}


ScalarField Expo(const MultiLayerMap& m)
{
    ScalarField res = m.generate_field();
    int nb_dir = 100;
    int nb_step = 15;
    double total = nb_dir*3.1415/2.0;
    for(int j = 0; j < res.grid_height(); ++j)
	{
		for(int i = 0; i < res.grid_width(); ++i)
		{
            double val = m.get_sum(i, j);
            double sum_exp = 0;
            for(int d = 0; d < nb_dir; d++)
            {
                double angle = (2.*3.1415)*d/nb_dir;
                Eigen::Vector2d delta_pos = {cos(angle), sin(angle)};
                double covA = 0;
                double h = 0;
                for(int s = 0; s < nb_step; ++s)
                {
                    double v = m.get_sum(i+s*delta_pos(0), j+s*delta_pos(1)) - val;
                    if (v > 0)
                    {
                        double tmpCov = atan(v/delta_pos.norm());
                        if(tmpCov > covA)
                        {
                            covA = tmpCov;
                        }
                    }
                }
                sum_exp += (3.1415/2.0)-covA;
            }
            res.at(i, j) = sum_exp/total;
		}
	}

    return res;
}