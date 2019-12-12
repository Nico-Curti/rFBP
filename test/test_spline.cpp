#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <spline.h>
#include <cmath>

#define PRECISION 1e-4
#define SEED 42

#define isclose(x, y) ( std :: fabs((x) - (y)) < PRECISION )

TEST_CASE ( "Test spline interpolation", "[spline]")
{
  const int N = 10;
  double * X = new double[N];
  double * Y = new double[N];

  for (int i = 0; i < N; ++i) X[i] = 1. + i * .1;
  for (int i = 0; i < N; ++i) Y[i] = std :: atanh(std :: erf(X[i]));

  spline s;
  s.set_boundary(spline :: second_deriv, -20., spline :: second_deriv, -20., false);

  s.set_points(X, Y, N);

  REQUIRE ( isclose( s(X[0]), 1.23042 ) );
  REQUIRE ( isclose( s(X[1]), 1.37668 ) );
  REQUIRE ( isclose( s(X[2]), 1.52935 ) );
  REQUIRE ( isclose( s(X[3]), 1.68891 ) );
  REQUIRE ( isclose( s(X[4]), 1.85576 ) );
  REQUIRE ( isclose( s(X[5]), 2.03027 ) );
  REQUIRE ( isclose( s(X[6]), 2.21279 ) );
  REQUIRE ( isclose( s(X[7]), 2.40358 ) );
  REQUIRE ( isclose( s(X[8]), 2.6029  ) );
  REQUIRE ( isclose( s(X[9]), 2.81094 ) );

  REQUIRE ( isclose( s(1.234), 1.58353) );

  delete[] X;
  delete[] Y;
}
