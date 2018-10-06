#include "catch.hpp"

#include <Eigen/Core>

#include <ScalarField.hpp>

TEST_CASE("Test ScalarField functionment", "[ScalarField]")
{
    ScalarField sf({0, 0}, {1, 1}, 2, 2);
    sf.set_value(0, 0, 0);
    sf.set_value(0, 1, 0.5);
    sf.set_value(1, 0, 0.5);
    sf.set_value(1, 1, 1.0);
    
    REQUIRE(sf.get_value(0, 0) == 0);
    REQUIRE(sf.get_value(1, 1) == 1);
    REQUIRE(sf.get_value(0, 1) == 0.5);
    REQUIRE(sf.get_value(0.5, 0.5) == 0.5);
}