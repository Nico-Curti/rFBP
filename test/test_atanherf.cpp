#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <atanherf.h>
#include <cmath>
#include <random>

#define PRECISION 1e-4
#define SEED 42

#define isclose(x, y) ( std :: abs((x) - (y)) < PRECISION )

TEST_CASE ( "Test atanherf functions and approximations", "[atanherf]" )
{
  std :: mt19937 engine(SEED);
  std :: uniform_real_distribution < double > dist(4., 5.);

  for (int i = 0; i < 100; ++i)
  {
    const double rng_x = dist(engine);

    REQUIRE ( isclose( AtanhErf :: atanherf_largex(rng_x), std :: atanh(std :: erf(rng_x)) ) );
    REQUIRE ( isclose( sign(rng_x) * AtanhErf :: atanherf_interp(std :: abs(rng_x)), std :: atanh(std :: erf(rng_x)) ) );
  }
}

