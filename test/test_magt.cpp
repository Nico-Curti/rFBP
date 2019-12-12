#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <magnetization.h>
#include <cmath>
#include <random>

#define PRECISION 1e-4
#define SEED 42

#define isclose(x, y) ( std :: fabs((x) - (y)) < PRECISION )


TEST_CASE ( "Test MagT64 algebra", "[MaTP64]" )
{
  std :: mt19937 engine(SEED);
  std :: uniform_real_distribution < double > dist(0., 10.);

  for (int i = 0; i < 100; ++i)
  {
    const double x = dist(engine);
    const double y = dist(engine);

    MagT64 m1(x);
    MagT64 m2(y);
    MagT64 null(0.);

    // check value
    REQUIRE ( (isclose(m1.mag, x) && isclose(m2.mag, y) && isclose(m1.value(), std :: tanh(x)) && isclose(m2.value(), std :: tanh(y))) );

    // check copy constructor
    MagT64 m11(x);
    REQUIRE ( (isclose(m1.mag, m11.mag) && isclose(m1.value(), m11.value())) );

    // check assignment operator
    MagT64 m12 = m1;
    REQUIRE ( (isclose(m1.mag, m12.mag) && isclose(m1.value(), m12.value())) );

    // check mag format
    std :: string fmt = m1.magformat();
    REQUIRE ( fmt == "tanh" );

    // check % operator
    MagT64 op11 = m1 % m2;
    MagT64 op12 = m2 % m1;
    MagT64 op13 = m1 % null;
    REQUIRE ( (isclose(op11.mag, op12.mag) && isclose(op11.value(), op12.value())) );
    REQUIRE ( (isclose(op13.mag, m1.mag) && isclose(op13.value(), m1.value())) );

    // check + operator
    MagT64 op21 = m1 + m2;
    MagT64 op22 = m2 + m1;
    MagT64 op23 = m1 + null;
    REQUIRE ( (isclose(op21.mag, op22.mag) && isclose(op21.value(), op22.value())) );
    REQUIRE ( (isclose(op23.mag, m1.mag) && isclose(op23.value(), m1.value())) );

    // check / operator
    MagT64 op31 = m1 / y;
    MagT64 op32 = m1 / x;
    MagT64 op33 = m1 / 0.;
    REQUIRE ( (isclose(op31.mag, m1.mag / y) && isclose(op31.value(), std :: tanh(m1.mag / y))) );
    REQUIRE ( (isclose(op32.mag, 1.) && isclose(op32.value(), std :: tanh(1.))) );
    REQUIRE ( (op33.mag == INF && isclose(op33.value(), 1.)) );

    // check * operator
    // double op41 = m1 * y;
    // double op42 = m1 * 1.;
    // double op43 = m1 * 0.;
    // REQUIRE ( (op41 == m1.mag * y && op41 == m1.value() * y) );
    // REQUIRE ( (op42 == m1.mag && op42 == m1.value()) );
    // REQUIRE ( (op43 == 0.) );

    double op51 = y  * m1;
    double op52 = 1. * m1;
    double op53 = 0. * m1;
    REQUIRE ( (isclose(op51, m1.mag * y) && isclose(m1.value(), std :: tanh(op51 / y))) );
    REQUIRE ( (isclose(op52, m1.mag) && isclose(m1.value(), std :: tanh(op52))) );
    REQUIRE ( (isclose(op53, 0.)) );

    // check ^ operator
    MagT64 op61 = m1 ^ m2;
    MagT64 op62 = m2 ^ m1;
    MagT64 op63 = m1 ^ null;
    // MagT64 op64 = m1 ^ MagT64(1.);
    REQUIRE ( (isclose(op61.mag, op62.mag) && isclose(op61.value(), op62.value())) );
    REQUIRE ( (isclose(op63.mag, 0.) && isclose(op63.value(), 0.)) );
    // REQUIRE ( (op64.mag == m1.mag && op64.value() == m1.value()) );

    // check - operator
    double op71 = m1 - m2;
    double op72 = m2 - m1;
    double op73 = m1 - null;
    REQUIRE ( (isclose(m1.value(), op71 + m2.value())) );
    REQUIRE ( (isclose(m2.value(), op72 + m1.value())) );
    REQUIRE ( (isclose(op73, m1.value())) );

    // check == operator and != operator
    bool op81 = m1 == m2;
    bool op82 = m2 == m1;
    bool op83 = m1 != m2;
    bool op84 = m2 != m1;
    REQUIRE ( (op81 == op82 && op83 == op84) );
    REQUIRE ( (m1 == m11 && m1 == m12) );
  }

}

