#include <Utils.hpp>

#include <algorithm>
#include <vector>

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

double median_array(int n, double* values){
	std::vector<double> sorted_values;
	
	for(int i = 0; i != n; ++i){
		sorted_values.push_back(values[i]);
	}
	std::sort(sorted_values.begin(), sorted_values.end());
		
	if(n % 2 == 1){
		return sorted_values[n / 2 + 1];
	}else{
		return (sorted_values[n / 2] + sorted_values[n / 2 + 1]) * 0.5;
	}
}	

double abs_array(int n, double* values){
	for(int i = 0; i != n; ++i){
		values[i] = std::abs(values[i]);
	}
}

double mean_array(int n, double* values){
	double sum = 0.;
	for(int i = 0; i != n; ++i){
		sum += values[i];
	}

	return sum / n;	
}
