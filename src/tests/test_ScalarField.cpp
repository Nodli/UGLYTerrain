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

TEST_CASE("Test ScalarField equality", "[ScalarField]")
{
    ScalarField sf({0, 0}, {1, 1}, 2, 2);
    sf.set_value(0, 0, 0);
    sf.set_value(0, 1, 0.5);
    sf.set_value(1, 0, 0.5);
    sf.set_value(1, 1, 1.0);

    SECTION("Copy constructor is working")
    {
        ScalarField sf2(sf);

        REQUIRE(sf2.get_value(0.5, 0.5) == 0.5);
    }

    SECTION("Move constructor is working")
    {
        ScalarField sf2(std::move(sf));

        REQUIRE(sf2.get_value(0.5, 0.5) == 0.5);
    }

    SECTION("Affectation is working")
    {
        ScalarField sf2({1, 1}, {5, 5}, 8, 8);
        sf2 = sf;

        REQUIRE(sf2.cell_number() == sf.cell_number());
        REQUIRE(sf2.get_value(0.5, 0.5) == 0.5);
    }

    SECTION("Move is working")
    {
        ScalarField sf2({1, 1}, {5, 5}, 8, 8);
        sf2 = std::move(sf);

        REQUIRE(sf2.cell_number() == sf.cell_number());
        REQUIRE(sf2.get_value(0.5, 0.5) == 0.5);
    }
}