#include "catch.hpp"

#include <Eigen/Core>

#include <ScalarField.hpp>

TEST_CASE("Test ScalarField functionment", "[ScalarField]")
{
    ScalarField sf(2, 2);
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
    ScalarField sf(2, 2);
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
        ScalarField sf2(8, 8, {1, 1}, {5, 5});
        sf2 = sf;

        REQUIRE(sf2.cell_number() == sf.cell_number());
        REQUIRE(sf2.get_value(0.5, 0.5) == 0.5);
    }

    SECTION("Move is working")
    {
        ScalarField sf2(8, 8, {1, 1}, {5, 5});
        sf2 = std::move(sf);

        REQUIRE(sf2.cell_number() == sf.cell_number());
        REQUIRE(sf2.get_value(0.5, 0.5) == 0.5);
    }

    SECTION("Value copy is working")
    {
        ScalarField sf2(2, 2, {0, 0}, {2, 2});
        sf2.copy_values(sf);

        REQUIRE(sf2.get_value(1.0, 1.0) == 0.5);
    }

    SECTION("Value Moving is working")
    {
        ScalarField sf2(2, 2, {0, 0}, {2, 2});
        sf2.copy_values(std::move(sf));

        REQUIRE(sf2.get_value(1.0, 1.0) == 0.5);
    }

    SECTION("Value copy will throw is not the right size")
    {
        ScalarField sf2(5, 5, {0, 0}, {2, 2});
        REQUIRE_THROWS(sf2.copy_values(sf));
    }
}