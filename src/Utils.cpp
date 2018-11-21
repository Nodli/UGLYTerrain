#include <Utils.hpp>

void proportion(int n, double values[], double proportions[]){

	double sum = 0;
	for(int ivalue = 0; ivalue < n; ++ivalue){
		sum += values[ivalue];
	}

	for(int ivalue = 0; ivalue < n; ++ivalue){
		proportions[ivalue] = values[ivalue] / sum;
	}

}