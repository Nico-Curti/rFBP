#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <fprotocol.h>

#define PRECISION 1e-4f

TEST_CASE ( "Test different focusing protocol", "[fprotocol]" )
{
  const long int max_steps = 101;

  SECTION ("Test Scoping protocol")
  {
    FocusingProtocol fp("scoping", max_steps);

    // check size
    REQUIRE ( fp.Nrep == max_steps );

    // check starting and last values
    REQUIRE ( fp.gamma[0] == 0.  );
    REQUIRE ( fp.gamma[max_steps - 1L] == INF );
    REQUIRE ( fp.n_rep[0] == 21. ); // 21 is a magic number ??

    // check increment range
    // check all values equal
    for (long int i = 0L; i < max_steps - 2L; ++i)
    {
      REQUIRE ( fp.gamma[i] < fp.gamma[i + 1L] );
      REQUIRE ( fp.n_rep[i] == fp.n_rep[i + 1L] );
      REQUIRE ( fp.beta[i]  == fp.beta[i + 1L] );
    }


    REQUIRE ( fp.beta[0] == INF );
  }

  SECTION ("Test Pseudo Reinforcement protocol")
  {
    FocusingProtocol fp("pseudo_reinforcement", max_steps);

    // check size
    REQUIRE ( fp.Nrep == max_steps );

    // check starting and last values
    REQUIRE ( fp.gamma[0] == 0. );
    REQUIRE ( fp.gamma[max_steps - 1L] == INF );
    REQUIRE ( fp.n_rep[0] == 1. );

    // check increment range
    // check all values equal
    for (long int i = 0L; i < max_steps - 2L; ++i)
    {
      REQUIRE ( fp.gamma[i] < fp.gamma[i + 1L] );
      REQUIRE ( fp.n_rep[i] < fp.n_rep[i + 1L] );
      REQUIRE ( fp.beta[i] == fp.beta[i + 1L] );
    }

    REQUIRE ( fp.beta[0] == INF );
  }

  SECTION ("Test Free Scoping protocol")
  {
    FocusingProtocol fp("free_scoping", max_steps);

    // check size
    REQUIRE ( fp.Nrep == max_steps );

    // check starting and last values
    REQUIRE ( fp.gamma[0] == 1. );
    REQUIRE ( fp.gamma[max_steps - 1L] == INF );
    REQUIRE ( fp.n_rep[0] == 2. );
    REQUIRE ( fp.n_rep[max_steps - 1L] == INF );

    // check increment range
    // check all values equal
    for (long int i = 0L; i < max_steps - 2L; ++i)
    {
      REQUIRE ( fp.gamma[i] < fp.gamma[i + 1L] );
      REQUIRE ( fp.n_rep[i] < fp.n_rep[i + 1L] );
      REQUIRE ( fp.beta[i] == fp.beta[i + 1L] );
    }

    REQUIRE ( fp.beta[0] == INF );
  }

  SECTION ("Test Standard Reinforcement protocol")
  {
    FocusingProtocol fp("standard_reinforcement", max_steps);

    // check size
    REQUIRE ( fp.Nrep == max_steps );

    // check starting and last values
    REQUIRE ( fp.n_rep[0] == 1. );
    REQUIRE ( fp.gamma[0] == INF );
    REQUIRE ( fp.beta[0]  == INF );

    // check increment range
    // check all values equal
    for (long int i = 0L; i < max_steps - 2L; ++i)
    {
      REQUIRE ( fp.n_rep[i] < fp.n_rep[i + 1L] );
      REQUIRE ( fp.gamma[i] == fp.gamma[i + 1L] );
      REQUIRE ( fp.beta[i] == fp.beta[i + 1L] );
    }

  }
}

