#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <pattern.h>
#include <algorithm>


TEST_CASE ( "Test Pattern object", "[pattern]" )
{

  SECTION ("Test Random pattern")
  {
    const int N = 10;
    const int M = 20;

    Patterns pt(N, M);

    // check size
    REQUIRE ( pt.Nrow == N );
    REQUIRE ( pt.Ncol == M );
    REQUIRE ( pt.Nout == N );

    // check binary values
    for (long int i = 0L; i < N; ++i)
    {
      REQUIRE ( (pt.output[i] == 1L || pt.output[i] == -1L) );

      for (long int j = 0L; j < M; ++j)
        REQUIRE ( (pt.input[i][j] == 1L || pt.input[i][j] == -1L) );
    }
  }

  SECTION ("Test constructor from data")
  {
    const int N = 10;
    const int M = 20;

    double ** data = new double * [N];
    long int * label = new long int [N];

    std :: generate_n(data, N, [&](){return new double[M];});
    std :: fill_n(label, N, 1L);

    for (long int i = 0L; i < N; ++i)
      std :: fill_n(data[i], M, -1L);

    Patterns pt(data, label, N, M);

    // check size
    REQUIRE ( pt.Nrow == N );
    REQUIRE ( pt.Ncol == M );
    REQUIRE ( pt.Nout == N );

    // check binary values
    for (long int i = 0L; i < N; ++i)
    {
      REQUIRE ( pt.output[i] == label[i] );

      for (long int j = 0L; j < M; ++j)
        REQUIRE ( pt.input[i][j] == data[i][j] );
    }

    for (long int i = 0L; i < N; ++i) delete[] data[i];
    delete[] data;

    delete[] label;
  }

  SECTION ("Test copy constructor")
  {
    const int N = 10;
    const int M = 20;

    Patterns pt_1(N, M);
    Patterns pt_2(pt_1);

    // check size
    REQUIRE ( pt_1.Nrow == pt_2.Nrow );
    REQUIRE ( pt_1.Ncol == pt_2.Ncol );
    REQUIRE ( pt_1.Nout == pt_2.Nout );

    // check binary values
    for (long int i = 0L; i < N; ++i)
    {
      REQUIRE ( pt_1.output[i] == pt_2.output[i] );

      for (long int j = 0L; j < M; ++j)
        REQUIRE ( pt_1.input[i][j] == pt_2.input[i][j] );
    }
  }

  SECTION ("Test operator constructor")
  {
    const int N = 10;
    const int M = 20;

    Patterns pt_1(N, M);
    Patterns pt_2 = pt_1;

    // check size
    REQUIRE ( pt_1.Nrow == pt_2.Nrow );
    REQUIRE ( pt_1.Ncol == pt_2.Ncol );
    REQUIRE ( pt_1.Nout == pt_2.Nout );

    // check binary values
    for (long int i = 0L; i < N; ++i)
    {
      REQUIRE ( pt_1.output[i] == pt_2.output[i] );

      for (long int j = 0L; j < M; ++j)
        REQUIRE ( pt_1.input[i][j] == pt_2.input[i][j] );
    }
  }

}

