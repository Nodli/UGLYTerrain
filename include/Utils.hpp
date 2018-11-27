#pragma once

/**
 * @brief Computes the proportions of each value
 *
 * @param n         	the number of values
 * @param values		the values to compute the proportions
 * @param proportions	computed proportions
 */
void proportion(int n, double* values, double* proportions);

/**
 * @brief Computes the maximal value among values
 *
 * @param n         the number of values
 * @param values	the values to compute the proportions
 * @return double 	maximal value in values
 */
double max_array(int n, double* values);

/**
 * @brief Computes the minimal value among values
 *
 * @param n         the number of values
 * @param values	the values to compute the proportions
 * @return double 	minimal value in values
 */
double min_array(int n, double* values);