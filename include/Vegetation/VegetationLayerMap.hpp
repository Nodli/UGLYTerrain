#pragma once
#include <Grid2d.hpp>
#include <vector>

class Plant
{
public:
	int _age = 0;
	int _max_age = 70;
	int _reproduction_age = 20;
	float _health = 1.0;
};

class VegetationLayerMap : public Grid2d
{
public:
	VegetationLayerMap() = delete;
	/**
	 * @brief Construct a new Vegetation layer from an existing vegetation layer
	 *
	 * @param hf        the layer to copy
	 */
	VegetationLayerMap(const VegetationLayerMap& vl)
		: Grid2d(vl), _cells(vl._cells) {}
	/**
	 * @brief Construct a new Vegetation layer from an existing vegetation layer
	 *
	 * @param hf        the layer to copy
	 */
	VegetationLayerMap(VegetationLayerMap&& vl)
		: Grid2d(std::move(vl)), _cells(std::move(vl._cells)) {}

	/**
	 * @brief Construct a new empty Vegetation layer from a grid
	 *
	 * @param g         the initial grid of the layer
	 */
	VegetationLayerMap(const Grid2d &g)
		: Grid2d(g)
	{
		_cells.resize(g.cell_number());
	}
	/**
	 * @brief Construct a new Vegetation layer from a box
	 *
	 * @param b         the box in wich to create the grid
	 * @param width     the number of cells along the width of the grid
	 * @param height    the number of cells along the height of the grid
	 */
	VegetationLayerMap(const Box2d &b, const int width, const int height)
		: Grid2d(b, width, height)
	{
		_cells.resize(width * height);
	}
	/**
	 * @brief Construct a new Vegetation layer object
	 *
	 * @param width     the number of cells along the width of the grid
	 * @param height    the number of cells along the height of the grid
	 * @param a         the first point of the grid
	 * @param b         the second point of the grid
	 */
	VegetationLayerMap(const int width, const int height, const Eigen::Vector2d a = {0, 0}, const Eigen::Vector2d b = {1, 1})
		: Grid2d(width, height, a, b)
	{
		_cells.resize(width * height);
	}

    std::vector<Plant>& at(int i, int j)
    {
        return _cells.at(index(i, j));
    }

private:
	std::vector<std::vector<Plant>> _cells;
};