#include <Utils.hpp>

void proportion(const int n, double* values, double* proportions){

	double sum = 0;

	for(int ivalue = 0; ivalue < n; ++ivalue){
		sum += values[ivalue];
	}

	for(int ivalue = 0; ivalue < n; ++ivalue){
		if(sum != 0.){
			proportions[ivalue] = values[ivalue] / sum;
		}else{
			proportions[ivalue] = 0.;
		}
	}

}

double max_array(int n, double* values){

	double max = values[0];

	for(int ivalue = 1; ivalue < n; ++ivalue){
		if(values[ivalue] > max){
			max = values[ivalue];
		}
	}

	return max;

}

double min_array(int n, double* values){

	double max = values[0];

	for(int ivalue = 1; ivalue < n; ++ivalue){
		if(values[ivalue] < max){
			max = values[ivalue];
		}
	}

	return max;

}