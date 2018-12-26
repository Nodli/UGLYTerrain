#include "catch.hpp"

#include <Eigen/Core>

#include <ScalarField.hpp>
#include <Noise/TerrainNoise.hpp>

TEST_CASE("Test ScalarField functionment", "[ScalarField]")
{
	ScalarField sf(2, 2);
	sf.set_value(0, 0, 0);
	sf.set_value(0, 1, 0.5);
	sf.set_value(1, 0, 0.5);
	sf.set_value(1, 1, 1.0);
	REQUIRE(sf.value(0, 0) == 0);
	REQUIRE(sf.value(1, 1) == 1);
	REQUIRE(sf.value(0, 1) == 0.5);
	REQUIRE(sf.value_inter(0.5, 0.5) == 0.5);
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
		REQUIRE(sf2.value_inter(0.5, 0.5) == 0.5);
	}
	SECTION("Move constructor is working")
	{
		ScalarField sf2(std::move(sf));
		REQUIRE(sf2.value_inter(0.5, 0.5) == 0.5);
	}
	SECTION("Affectation is working")
	{
		ScalarField sf2(8, 8, {1, 1}, {5, 5});
		sf2 = sf;
		REQUIRE(sf2.cell_number() == sf.cell_number());
		REQUIRE(sf2.value_inter(0.5, 0.5) == 0.5);
	}
	SECTION("Move is working")
	{
		ScalarField sf2(8, 8, {1, 1}, {5, 5});
		sf2 = std::move(sf);
		REQUIRE(sf2.cell_number() == sf.cell_number());
		REQUIRE(sf2.value_inter(0.5, 0.5) == 0.5);
	}
	SECTION("Value copy is working")
	{
		ScalarField sf2(2, 2, {0, 0}, {2, 2});
		sf2.copy_values(sf);
		REQUIRE(sf2.value_inter(1.0, 1.0) == 0.5);
	}
	SECTION("Value Moving is working")
	{
		ScalarField sf2(2, 2, {0, 0}, {2, 2});
		sf2.copy_values(std::move(sf));
		REQUIRE(sf2.value_inter(1.0, 1.0) == 0.5);
	}
	SECTION("Value copy will throw is not the right size")
	{
		ScalarField sf2(5, 5, {0, 0}, {2, 2});
		REQUIRE_THROWS(sf2.copy_values(sf));
	}
}

TEST_CASE("Test ScalarField addition", "[ScalarField]")
{
	ScalarField sf1(2, 2);
	sf1.set_value(0, 0, 0);
	sf1.set_value(0, 1, 0.25);
	sf1.set_value(1, 0, 0.5);
	sf1.set_value(1, 1, 1.0);

	ScalarField sf2(2, 2);
	sf2.set_value(0, 0, 1.0);
	sf2.set_value(0, 1, 1.0);
	sf2.set_value(1, 0, 1.0);
	sf2.set_value(1, 1, 1.0);

	SECTION("Addition give the good result")
	{
		ScalarField result = sf1 + sf2;
		REQUIRE(result.value(0, 0) == 1.0);
		REQUIRE(result.value(0, 1) == 1.25);
		REQUIRE(result.value(1, 0) == 1.5);
		REQUIRE(result.value(1, 1) == 2.0);
	}

	SECTION("Addition preserve added elements")
	{
		sf1 += sf2 + sf1 + sf2;
		REQUIRE(sf1.value(0, 0) == 2.0);
		REQUIRE(sf1.value(0, 1) == 2.5);
		REQUIRE(sf1.value(1, 0) == 3.0);
		REQUIRE(sf1.value(1, 1) == 4.0);

		REQUIRE(sf2.value(0, 0) == 1.0);
		REQUIRE(sf2.value(0, 1) == 1.0);
		REQUIRE(sf2.value(1, 0) == 1.0);
		REQUIRE(sf2.value(1, 1) == 1.0);
	}
}

TEST_CASE("Test if Obj exporter", "[ScalarField]")
{
	ScalarField sf(2, 2);
	sf.set_value(0, 0, 0);
	sf.set_value(0, 1, 0.5);
	sf.set_value(1, 0, 0.5);
	sf.set_value(1, 1, 1.0);
	sf.export_as_obj("test.obj");
	sf.export_as_obj("test2.obj", "potato");
	sf = ScalarField(3, 3, { -5, -5}, {5, 5});
	sf.set_value(0, 0, 0);
	sf.set_value(0, 1, 2);
	sf.set_value(0, 2, 1);
	sf.set_value(1, 0, 0.5);
	sf.set_value(1, 1, 3);
	sf.set_value(1, 2, 1);
	sf.set_value(2, 0, 0);
	sf.set_value(2, 1, 1);
	sf.set_value(2, 2, 1.5);
	sf.export_as_obj("test3.obj");
}

TEST_CASE("Test if pgm exporter and slope_map getter works", "[ScalardField]")
{
	ScalarField sf(100, 100, { -25, -25}, {25, 25});
	TerrainNoise t_noise(10.0, 1.0 / 200.0, 8);

	for(int j = 0; j < 100; ++j)
	{
		for(int i = 0; i < 100; i++)
		{
			sf.at(i, j) = t_noise.get_noise(i, j);
		}
	}

	sf.export_as_obj("Test_pgm.obj");
	sf.export_as_pgm("Test_pgm.pgm");
	ScalarField::generate_slope_map(sf).export_as_pgm("Test_pgm_slope.pgm");
	ScalarField::generate_slope_map(sf).export_as_obj("Test_pgm_slope.obj");
}