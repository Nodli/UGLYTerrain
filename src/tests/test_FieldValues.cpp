#include "catch.hpp"

#include <Eigen/Core>

#include <FieldValues.hpp>

TEST_CASE("Test ParameterizedNoise functionment", "[ParameterizedNoise]")
{
    FastNoise n;
    n.SetNoiseType(FastNoise::Perlin);
    double ampl = 10.0;
    double wave = 440.0;
    ParameterizedNoise pn(n, ampl, wave);

    REQUIRE(pn.get_amplitude() == ampl);
    REQUIRE(pn.get_wave_length() == wave);
    REQUIRE(pn.get_noise_type() == FastNoise::Perlin);
}

TEST_CASE("Test FieldValues functionment", "[FieldValues]")
{
    FieldValues fv = FieldValues();
    fv.add_noise(FastNoise::Perlin, 10.0, 440.0);
    fv.compute_harmonics(0, 3);
    double sum = fv.compute_noises_sum(0, 0);

    REQUIRE(fv.get_noises_number() == 4);

    REQUIRE(fv.get_noise(0).get_wave_length() == 440.0);
    REQUIRE(fv.get_noise(1).get_wave_length() == 220.0);
    REQUIRE(fv.get_noise(2).get_wave_length() == 110.0);
    REQUIRE(fv.get_noise(3).get_wave_length() == 55.0);

    REQUIRE(fv.get_noise(0).get_amplitude() == 10.0);
    REQUIRE(fv.get_noise(1).get_amplitude() == 5.0);
    REQUIRE(fv.get_noise(2).get_amplitude() == 2.5);
    REQUIRE(fv.get_noise(3).get_amplitude() == 1.25);
}