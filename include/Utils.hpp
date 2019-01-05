#pragma once

/**
 * @brief Computes the proportions of each value
 *
 * @param n         	the number of values
 * @param values	the values to compute the proportions
 * @param proportions	computed proportions
 */
void proportion(int n, double* values, double* proportions);

/**
 * @brief Computes the maximal value among values
 *
 * @param n         	the number of values
 * @param values	the values to compute the maximum
 * @return double 	maximal value in values
 */
double max_array(int n, double* values);

/**
 * @brief Computes the minimal value among values
 *
 * @param n         	the number of values
 * @param values	the values to compute the minimum 
 * @return double 	minimal value in values
 */
double min_array(int n, double* values);

/**
 * @brief Computes the median value among values
 *
 * @param n         	the number of values
 * @param values	the values to compute the median
 * @return double 	median value of values
 */
double median_array(int n, double* values);

/**
 * @brief Modifies each value of values to its absolute value
 *
 * @param n         	the number of values
 * @param values	the values to modify
 */
void abs_array(int n, double* values);

/**
 * @brief Computes the mean value among values 
 *
 * @param n         	the number of values
 * @param values	the values to compute the mean 
 * @return double 	mean value of values
 */
double mean_array(int n, double* values);

/**
 * @brief Modifies each value of values to its opposite
 *
 * @param n         	the number of values
 * @param values	the values to modify 
 */
void opp_array(int n, double* values);
