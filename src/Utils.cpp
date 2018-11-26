#include <Utils.hpp>

double proportion(const int n, double* values, double* proportions){

	double sum = 0;
	double max_value = values[0];
	for(int ivalue = 0; ivalue != n; ++ivalue){
		sum += values[ivalue];

		if(values[ivalue] > max_value){
			max_value = values[ivalue];
		}
	}

	for(int ivalue = 0; ivalue < n; ++ivalue){
		proportions[ivalue] = values[ivalue] / sum;
	}

	return max_value;

}