#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <magnetization.h>
#include <cmath>
#include <random>

#define PRECISION 1e-4
#define SEED 42

#define isclose(x, y) ( std :: fabs((x) - (y)) < PRECISION )


TEST_CASE ( "Test MagP64 algebra", "[MagP64]" )
{
  std :: mt19937 engine(SEED);
  std :: uniform_real_distribution < double > dist(0., 10.);

  for (int i = 0; i < 100; ++i)
  {
    const double x = dist(engine);
    const double y = dist(engine);

    MagP64 m1(x);
    MagP64 m2(y);
    MagP64 null(0.);

    // check value
    REQUIRE ( (isclose(m1.mag, x) && isclose(m2.mag, y) && isclose(m1.value(), x) && isclose(m2.value(), y)) );

    // check copy constructor
    MagP64 m11(x);
    REQUIRE ( (isclose(m1.mag, m11.mag) && isclose(m1.value(), m11.value())) );

    // check assignment operator
    MagP64 m12 = m1;
    REQUIRE ( (isclose(m1.mag, m12.mag) && isclose(m1.value(), m12.value())) );

    // check mag format
    std :: string fmt = m1.magformat();
    REQUIRE ( fmt == "plain" );

    // check % operator
    MagP64 op11 = m1 % m2;
    MagP64 op12 = m2 % m1;
    MagP64 op13 = m1 % null;
    REQUIRE ( (isclose(op11.mag, op12.mag) && isclose(op11.value(), op12.value())) );
    REQUIRE ( (op11.mag >= -1. && op11.mag <= 1.) );
    REQUIRE ( (op12.mag >= -1. && op12.mag <= 1.) );
    REQUIRE ( (op11.value() >= -1. && op11.value() <= 1.) );
    REQUIRE ( (op12.value() >= -1. && op12.value() <= 1.) );
    REQUIRE ( (isclose(op13.mag, std :: clamp(m1.mag, -1., 1.)) && isclose(op13.value(), std :: clamp(m1.value(), -1., 1.))) );

    // check + operator
    MagP64 op21 = m1 + m2;
    MagP64 op22 = m2 + m1;
    MagP64 op23 = m1 + null;
    REQUIRE ( (isclose(op21.mag, op22.mag) && isclose(op21.value(), op22.value())) );
    REQUIRE ( (isclose(op23.mag, m1.mag) && isclose(op23.value(), m1.value())) );

    // check / operator
    MagP64 op31 = m1 / y;
    MagP64 op32 = m1 / x;
    MagP64 op33 = m1 / 0.;
    REQUIRE ( (isclose(op31.mag, m1.mag / y) && isclose(op31.value(), m1.value() / y)) );
    REQUIRE ( (isclose(op32.mag, 1.) && isclose(op32.value(), 1.)) );
    REQUIRE ( (op33.mag == INF && op33.value() == INF) );

    // check * operator
    double op41 = m1 * y;
    double op42 = m1 * 1.;
    double op43 = m1 * 0.;
    REQUIRE ( (isclose(op41, m1.mag * y) && isclose(op41, m1.value() * y)) );
    REQUIRE ( (isclose(op42, m1.mag) && isclose(op42, m1.value())) );
    REQUIRE ( isclose(op43, 0.) );

    double op51 = y  * m1;
    double op52 = 1. * m1;
    double op53 = 0. * m1;
    REQUIRE ( (isclose(op51, m1.mag * y) && isclose(op51, m1.value() * y)) );
    REQUIRE ( (isclose(op52, m1.mag) && isclose(op52, m1.value())) );
    REQUIRE ( isclose(op53, 0.) );

    // check ^ operator
    MagP64 op61 = m1 ^ m2;
    MagP64 op62 = m2 ^ m1;
    MagP64 op63 = m1 ^ null;
    MagP64 op64 = m1 ^ MagP64(1.);
    REQUIRE ( (isclose(op61.mag, op62.mag) && isclose(op61.value(), op62.value())) );
    REQUIRE ( (isclose(op63.mag, 0.) && isclose(op63.value(), 0.)) );
    REQUIRE ( (isclose(op64.mag, m1.mag) && isclose(op64.value(), m1.value())) );

    // check - operator
    double op71 = m1 - m2;
    double op72 = m2 - m1;
    double op73 = m1 - null;
    REQUIRE ( (isclose(m1.mag, op71 + m2.mag) && isclose(m1.value(), op71 + m2.value())) );
    REQUIRE ( (isclose(m2.mag, op72 + m1.mag) && isclose(m2.value(), op72 + m1.value())) );
    REQUIRE ( (isclose(op73, m1.mag) && isclose(op73, m1.value())) );

    // check == operator and != operator
    bool op81 = m1 == m2;
    bool op82 = m2 == m1;
    bool op83 = m1 != m2;
    bool op84 = m2 != m1;
    REQUIRE ( (op81 == op82 && op83 == op84) );
    REQUIRE ( (m1 == m11 && m1 == m12) );
  }

}

