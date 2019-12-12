#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <params.hpp>
#include <algorithm>


TEST_CASE ( "Test Params object", "[params]" )
{

  SECTION ("Test Params object MagP64")
  {
    long int max_iters = 100;

    double damping = 0.1;
    double epsil = 0.2;
    double beta = 0.3;
    double r = 0.4;

    double tan_gamma = 3.14;

    std :: string accuracy1 = "none_1";
    std :: string accuracy2 = "none_2";

    Params < MagP64 > p(max_iters, damping, epsil, beta, r, tan_gamma, accuracy1, accuracy2);

    REQUIRE ( p.max_iters     == max_iters );
    REQUIRE ( p.damping       == damping   );
    REQUIRE ( p.epsil         == epsil     );
    REQUIRE ( p.beta          == beta      );
    REQUIRE ( p.r             == r         );
    REQUIRE ( p.tan_gamma.mag == tan_gamma );
    REQUIRE ( p.accuracy1     == accuracy1 );
    REQUIRE ( p.accuracy2     == accuracy2 );
  }

  SECTION ("Test Params object MagT64")
  {
    long int max_iters = 100;

    double damping = 0.1;
    double epsil = 0.2;
    double beta = 0.3;
    double r = 0.4;

    double tan_gamma = 3.14;

    std :: string accuracy1 = "none_1";
    std :: string accuracy2 = "none_2";

    Params < MagT64 > p(max_iters, damping, epsil, beta, r, tan_gamma, accuracy1, accuracy2);

    REQUIRE ( p.max_iters     == max_iters );
    REQUIRE ( p.damping       == damping   );
    REQUIRE ( p.epsil         == epsil     );
    REQUIRE ( p.beta          == beta      );
    REQUIRE ( p.r             == r         );
    REQUIRE ( p.tan_gamma.mag == tan_gamma );
    REQUIRE ( p.accuracy1     == accuracy1 );
    REQUIRE ( p.accuracy2     == accuracy2 );
  }

}
