#pragma once

#include <ScalarField.hpp>

class MultiLayerMap : public Grid2d
{
    MultiLayerMap() = delete;
    MultiLayerMap(const MultiLayerMap& map) : Grid2d(map), _fields(map._fields) {}
    MultiLayerMap(MultiLayerMap&& map) : Grid2d(std::move(map)), _fields(std::move(map._fields)) {}
    MultiLayerMap(const int width, const int height, const Eigen::Vector2d a = {0, 0}, const Eigen::Vector2d b = {1, 1})
        : Grid2d(width, height, a, b) {}

    const ScalarField& get_field(int field_index) const {return _fields.at(field_index);}
    ScalarField& get_field(int field_index) {return _fields.at(field_index);}

    ScalarField generate_field() const;

    void set_value(int field_index, int i, int j, double v) {_fields.at(field_index).set_value(i, j, v);}

    void set_field(int field_index, const ScalarField& field){_fields.at(field_index).copy_values(field);};
    void set_field(int field_index, ScalarField&& field){_fields.at(field_index).copy_values(std::move(field));}

    void add_field(const ScalarField& field){_fields.push_back(field);}
    void add_field(ScalarField&& field){_fields.push_back(std::move(field));}

    void reshape(double ax, double ay, double bx, double by);
    void reshape(const Eigen::Vector2d a, const Eigen::Vector2d b);

    MultiLayerMap& operator=(const MultiLayerMap& mlm);
    MultiLayerMap& operator=(MultiLayerMap&& mlm);

private:
    std::vector<ScalarField> _fields; 
};