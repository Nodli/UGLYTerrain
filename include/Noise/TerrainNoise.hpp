#include <FastNoise/FastNoise.hpp>

/**
 * @brief Fractal ridged noise generator
 *
 */
class TerrainNoise
{
public:
	/**
	 * @brief Construct a new Terrain Noise object
	 *
	 * @param ampl      initial amplitude of the noise
	 * @param freq      initial frequency of the noise
	 * @param oct       number of iteration of the noise
	 * @param s1        seed for the initial noise
	 * @param s2        seed for the ridge noise
	 */
	TerrainNoise(double ampl, double freq, int oct, int s1 = 0, int s2 = 4) :
		_amplitude(ampl), _base_freq(freq), _octaves(oct)
	{
		_base_noise.SetSeed(s1);
		_base_noise.SetNoiseType(FastNoise::Perlin);
		_ridge_noise.SetSeed(s2);
		_ridge_noise.SetNoiseType(FastNoise::Perlin);
	}

	/**
	 * @brief Get the value of the noise to a given point
	 *
	 * @param i, j      position to generate the value
	 * @return double   the value of the noise
	 */
	double get_noise(int i, int j);

	FastNoise _base_noise;      /**< base noise generator*/
	FastNoise _ridge_noise;     /**< noise generator used to generate ridges*/
	double _amplitude;          /**< initial amplitude of the noise*/
	double _base_freq;          /**< initial frequency of the noise*/
	int _octaves;               /**< number of iteration of the noise*/
};