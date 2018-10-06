#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <Eigen/Core>

#include <Box2d.hpp>

TEST_CASE("Test point in box", "[Box2d]")
{
    Box2d box(0.5, 0.5, 1, 1);
    Eigen::Vector2d p(0.0, 0.0);
    REQUIRE(!box.contains(p));
    p = Eigen::Vector2d(0.5, 0.5);
    REQUIRE(!box.contains(p));
    p = Eigen::Vector2d(0.75, 0.75);
    REQUIRE(box.contains(p));
    p = Eigen::Vector2d(1.75, 0.75);
    REQUIRE(!box.contains(p));
    p = Eigen::Vector2d(0.75, 1.75);
    REQUIRE(!box.contains(p));
    box = Box2d(1, 1, 0.5, 0.5);
    p = Eigen::Vector2d(0.0, 0.0);
    REQUIRE(!box.contains(p));
    p = Eigen::Vector2d(0.5, 0.5);
    REQUIRE(!box.contains(p));
    p = Eigen::Vector2d(0.75, 0.75);
    REQUIRE(box.contains(p));
    p = Eigen::Vector2d(1.75, 0.75);
    REQUIRE(!box.contains(p));
    p = Eigen::Vector2d(0.75, 1.75);
    REQUIRE(!box.contains(p));
}

TEST_CASE("Test box intersection", "[Box2d]")
{
    Box2d b1(0, 0, 1, 1);
    Box2d b2(0.5, 0.5, 0.1, 0.1);
    REQUIRE(b1.intersect(b2));
    REQUIRE(b2.intersect(b1));
    b2 = Box2d(0.5, 0.5, 1.5, 1.5);
    REQUIRE(b1.intersect(b2));
    REQUIRE(b2.intersect(b1));
    b2 = Box2d(-0.5, -0.5, 0.5, 0.5);
    REQUIRE(b1.intersect(b2));
    REQUIRE(b2.intersect(b1));
    b2 = Box2d(0.5, -0.5, 0.6, 1.5);
    REQUIRE(b1.intersect(b2));
    REQUIRE(b2.intersect(b1));
    b2 = Box2d(-0.5, -0.5, -0.6, 1.5);
    REQUIRE(!b1.intersect(b2));
    REQUIRE(!b2.intersect(b1));
    b2 = Box2d(0.5, -0.5, 0.6, -1.5);
    REQUIRE(!b1.intersect(b2));
    REQUIRE(!b2.intersect(b1));
    b2 = Box2d(1.5, 1.5, 1.6, 1.6);
    REQUIRE(!b1.intersect(b2));
    REQUIRE(!b2.intersect(b1));
}

TEST_CASE("Test box equality", "[Box2d]")
{
    Box2d b1(0, 0, 1, 1);

    SECTION("Copy constructor is working"){
        Box2d b2(b1);

        REQUIRE(b2.width() == b1.width());
        REQUIRE(b2.height() == b1.height());
    }

    SECTION("Move constructor is working"){
        Box2d b2(std::move(b1));

        REQUIRE(b2.width() == b1.width());
        REQUIRE(b2.height() == b1.height());
    }

    SECTION("Affectation is working"){
        Box2d b2(0, 0, 1.5, 1.5);
        b2 = b1;

        REQUIRE(b2.width() == b1.width());
        REQUIRE(b2.height() == b1.height());
    }

    SECTION("Move is working"){
        Box2d b2(0, 0, 1.5, 1.5);
        b2 = std::move(b1);

        REQUIRE(b2.width() == b1.width());
        REQUIRE(b2.height() == b1.height());
    }
}
