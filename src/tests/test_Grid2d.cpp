#include "catch.hpp"

#include <Eigen/Core>

#include <Grid2d.hpp>

TEST_CASE("Test Grid functionment", "[Grid2d]")
{
    Eigen::Vector2d a(0, 0);
    Eigen::Vector2d b(1, 1);
    Grid2d grid(a, b, 2, 2);
    REQUIRE(grid.position(0, 0) == a);
    REQUIRE(grid.position(1, 1) == Eigen::Vector2d(1, 1));
    Grid2d grid2(a, b, 3, 3);
    REQUIRE(grid2.position(1, 1) == Eigen::Vector2d(0.5, 0.5));
    REQUIRE(grid2.position(2, 2) == Eigen::Vector2d(1, 1));
}

TEST_CASE("Test grid get grid position", "[Grid2d]")
{
    Eigen::Vector2d a(0, 0);
    Eigen::Vector2d b(1, 1);
    Grid2d grid(a, b, 3, 3);

    REQUIRE(grid.grid_position(0.5, 0.5) == Eigen::Vector2i(1, 1));
    REQUIRE(grid.grid_position(0.25, 0.25) == Eigen::Vector2i(0, 0));
    REQUIRE(grid.grid_position(0.75, 0.75) == Eigen::Vector2i(1, 1));
}