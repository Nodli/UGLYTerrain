#pragma once

#include <SimpleLayerMap.hpp>

/**
 * @brief Defines a layered field
 *
 */
class MultiLayerMap : public DoubleField
{
public:
	using DoubleField::value;

	MultiLayerMap() = delete;
	/**
	 * @brief Construct a new Multi Layer Map object from an other one
	 *
	 * @param map       the Multi Layer Map to copy
	 */
	MultiLayerMap(const MultiLayerMap& map) : DoubleField(map), _layers(map._layers) {}
	/**
	 * @brief Construct a new Multi Layer Map object from an other one
	 *
	 * @param map       the Multi Layer Map to copy
	 */
	MultiLayerMap(MultiLayerMap&& map) : DoubleField(std::move(map)), _layers(std::move(map._layers)) {}
	
	MultiLayerMap(const Grid2d& d) : DoubleField(d) 
	{
	}
	/**
	 * @brief Construct a new Multi Layer Map object from scratch
	 *
	 * @param width     the number of cells along the width of the grid
	 * @param height    the number of cells along the height of the grid
	 * @param a         the first point of the grid
	 * @param b         the second point of the grid
	 */
	MultiLayerMap(const int width, const int height, const Eigen::Vector2d a = {0, 0}, const Eigen::Vector2d b = {1, 1})
		: DoubleField(width, height, a, b) {}

	/**
	 * @brief Get the value of the field at a given cell
	 *
	 * @param i, j      the position of the cell on the grid
	 * @return double   the sum of the values in every layer
	 */
	virtual double value(const int i, const int j) const;

	/**
	 * @brief Get the number of layers
	 *
	 * @return int      the number of layers
	 */
	int get_layer_number() const
	{
		return _layers.size();
	}

	/**
	 * @brief Get the a field of the Multi Layer Map
	 *
	 * @param field_index           the index of the field in the map
	 * @return const SimpleLayerMap&   a reference to the field
	 */
	const SimpleLayerMap& get_field(const int field_index) const
	{
		return _layers.at(field_index);
	}
	/**
	 * @brief Get the a field of the Multi Layer Map
	 *
	 * @param field_index           the index of the field in the map
	 * @return const SimpleLayerMap&   a modifiable reference to the field
	 */
	SimpleLayerMap& get_field(const int field_index)
	{
		return _layers.at(field_index);
	}

	/**
	 * @brief Generate an agregation of the Multi Layer Map.
	 * This is done by summing all the Scalar Fields together
	 *
	 * @return SimpleLayerMap      the Scalar Field sum of all the field in the Multi Layer Map
	 */
	SimpleLayerMap generate_field() const;

	/**
	 * @brief Get the sum of all values at one point.
	 * This is done by summing all the Scalar Fields together
	 *
	 * @return double	      	the double sum of all the values in the Multi Layer Map
	 */
	double get_sum(const int i, const int j) const;

	/**
	 * @brief Set the value of a field at a given position
	 *
	 * @param field_index       the index of the field to modify
	 * @param i, j              the position of the cell to modify
	 * @param v                 the new value to set
	 */
	void set_value(const int field_index, const int i, const int j, const double v)
	{
		_layers.at(field_index).set_value(i, j, v);
	}

	/**
	 * @brief Set the values of a whole field
	 *
	 * @param field_index       the index of the field to modify
	 * @param field             the field from which to copy the values
	 */
	void set_field(int field_index, const SimpleLayerMap& field)
	{
		_layers.at(field_index).copy_values(field);
	}
	/**
	 * @brief Set the values of a whole field
	 *
	 * @param field_index       the index of the field to modify
	 * @param field             the field from which to copy the values
	 */
	void set_field(int field_index, SimpleLayerMap&& field)
	{
		_layers.at(field_index).copy_values(std::move(field));
	}

	/**
	 * @brief Add a field to the Multi Layer Map
	 *
	 * @param field             the field to copy into the Multi Layer Map
	 */
	void add_field(const SimpleLayerMap& field)
	{
		_layers.push_back(field);
	}
	/**
	 * @brief Add a field to the Multi Layer Map
	 *
	 * @param field             the field to copy into the Multi Layer Map
	 */
	void add_field(SimpleLayerMap&& field)
	{
		_layers.push_back(std::move(field));
	}

	/**
	 * @brief Add a new layer to the map
	 *
	 * @return SimpleLayerMap&     a reference to the newly created layer
	 */
	SimpleLayerMap& new_layer();

	/**
	 * @brief Reshape the MultiLayerMap to a new size and position
	 *
	 * @param ax first x position
	 * @param ay first y position
	 * @param bx second x position
	 * @param by second y position
	 */
	void reshape(const double ax, const double ay, const double bx, const double by);
	/**
	 * @brief Reshape the MultiLayerMap to a new size and position
	 *
	 * @param a first point of the box
	 * @param b second point of the box
	 */
	void reshape(const Eigen::Vector2d a, const Eigen::Vector2d b);

	/**
	 * @brief Affectation operator
	 *
	 * @param mlm               the Multi Layer Map to affect
	 * @return MultiLayerMap&   a reference to this Multi Layer Map
	 */
	MultiLayerMap& operator=(const MultiLayerMap& mlm);
	/**
	 * @brief Affectation operator
	 *
	 * @param mlm               the Multi Layer Map to affect
	 * @return MultiLayerMap&   a reference to this Multi Layer Map
	 */
	MultiLayerMap& operator=(MultiLayerMap&& mlm);

	friend std::istream& operator>>(std::istream& is, MultiLayerMap& m);
	friend std::ostream& operator<<(std::ostream& os, const MultiLayerMap& m);

protected:
	std::vector<SimpleLayerMap> _layers; /**< Array of simple layer map*/
};

/**
 * @brief input flux operator
 * 
 * @param is 					the input stream to get the data from
 * @param m 					the target multilayermap
 * @return std::istream& 		the original input stream
 */
std::istream& operator>>(std::istream& is, MultiLayerMap& m);


/**
 * @brief output flux operator
 * 
 * @param os 					the output stream to save the data ta
 * @param m 					the multilayermap to save
 * @return std::ostream& 		the original output stream
 */
std::ostream& operator<<(std::ostream& os, const MultiLayerMap& m);

/**
 * @brief Normalizes the MultiLayerMap as a whole an not each SimpleLayerMap independently 
 *
 * @param mlm 				a MultiLayerMap to normalize
 * @return MultiLayerMap 	a normalized copy of mlm
 */
MultiLayerMap normalized(const MultiLayerMap& mlm);


