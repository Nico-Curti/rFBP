#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#if !defined __clang__ && __GNUC__ <= 6

  #include <magnetization_oldgcc.hpp>

#else

  #include <magnetization.hpp>

#endif

#include <params.hpp>
#include <rfbp.hpp>
#include <cavity_message.hpp>

#include <cmath>
#include <random>

#define PRECISION 1e-4
#define SEED 42
#define RANDFACT 0.1

#define isclose(x, y) ( std :: abs((x) - (y)) < PRECISION )

TEST_CASE ( "Test rfbp functions", "[rfbp]" )
{

  const long int K = 1;
  const long int M = 3;
  const long int N = 101;
  const double EPSIL = 0.9;
  const double DAMPING = 0.05;
  const long int MAX_ITERS = 2;
  const long int MAX_STEPS = 2;

  // Check theta update functions

  // MagP64
  MagVec2 < MagP64 > m_j_starP = new MagP64* [K];
  MagVec2 < MagP64 > m_inP     = new MagP64* [M];
  MagVec2 < MagP64 > m_noP     = new MagP64* [M];
  MagVec3 < MagP64 > weightsP  = new MagP64**[M];
  std :: generate_n(m_j_starP, K, [&]{return new MagP64 [N];});
  std :: generate_n(m_inP,     M, [&]{return new MagP64 [K];});
  std :: generate_n(m_noP,     M, [&]{return new MagP64 [K];});
  std :: generate_n(weightsP,  M, [&]{return new MagP64*[K];});

  // MagT64
  MagVec2 < MagT64 > m_j_starT = new MagT64* [K];
  MagVec2 < MagT64 > m_inT     = new MagT64* [M];
  MagVec2 < MagT64 > m_noT     = new MagT64* [M];
  MagVec3 < MagT64 > weightsT  = new MagT64**[M];
  std :: generate_n(m_j_starT, K, [&]{return new MagT64 [N];});
  std :: generate_n(m_inT,     M, [&]{return new MagT64 [K];});
  std :: generate_n(m_noT,     M, [&]{return new MagT64 [K];});
  std :: generate_n(weightsT,  M, [&]{return new MagT64*[K];});

  // Default indexes
  int k = 0;
  int a = 0;

  // Expected values
  double test_valueT = std::atanh( std::tanh(1) * DAMPING );
  double revDAMPING = 1. - DAMPING;
  double test2_valueT = std::atanh( 1. * revDAMPING );

  SECTION ( "TEST theta_node_update_exact" )
  {
        ////////////////
       // First test //
     /////////////////

    // MagP64
    std :: string accuracy = "exact";
    Params < MagP64 > paramsP(MAX_ITERS, DAMPING, EPSIL, static_cast < double >(NAN), 0., 0., accuracy, accuracy);

    for (long int i = 0L; i < K; ++i)
    {
      std :: fill_n(m_j_starP[i], N, 1L);
    }
    for (long int i = 0L; i < M; ++i)
    {
      std :: fill_n(m_inP[i], K, 1L);
      std :: fill_n(m_noP[i], K, 1L);
      for (long int j = 0L; j < K; ++j)
      {
        weightsP[i][j] = new MagP64[N];
        std :: fill_n(weightsP[i][j], N, 1L);
      }
    }
    double * inputsP  = new double[N];
    std :: fill_n(inputsP, N, 1L);
    std :: unique_ptr < MagP64[] > hP;
    hP.reset(new MagP64[N]);
    MagP64 old_m_onP = mag :: bar(m_inP[a][k], m_noP[a][k]);
    for (long int i = 0L; i < N; ++i)
    {
      hP[i] = mag :: bar(m_j_starP[k][i], weightsP[a][k][i]);
    }
    double maxdiffP = theta_node_update_exact(m_j_starP[k], m_inP[a][k], inputsP, weightsP[a][k], m_noP[a][k], paramsP, N, N);

    // This must be always true since I am changing only the messages from the factor node to the variables node
    MagP64 new_old_m_onP = mag :: bar(m_inP[a][k], m_noP[a][k]);
    REQUIRE ( isclose(old_m_onP.mag, new_old_m_onP.mag) );
    for (long int i = 0L; i < N; ++i)
    {
      MagP64 new_hP = mag :: bar(m_j_starP[k][i], weightsP[a][k][i]);
      REQUIRE ( isclose(hP[i].mag, new_hP.mag) );
    }

    // Beforehand they are all 1s, afterwards they are all 0s but thanks to the dumping function they are actually DAMPING*1

    REQUIRE ( isclose(m_inP[a][k].mag, DAMPING) );
    REQUIRE ( isclose(m_noP[a][k].mag, DAMPING) );
    for (long int i = 0L; i < N; ++i)
    {
      REQUIRE ( isclose(m_j_starP[k][i].mag, DAMPING) );
      REQUIRE ( isclose(weightsP[a][k][i].mag, DAMPING) );
    }

    // At the beginning m_noP are all 1s and afterwards, but before damping, 0s
    REQUIRE ( isclose(maxdiffP, 1.) );


    // MagT64
    Params < MagT64 > paramsT(MAX_ITERS, DAMPING, EPSIL, static_cast < double >(NAN), 0., 0., accuracy, accuracy);

    for (long int i = 0L; i < K; ++i)
    {
      std :: fill_n(m_j_starT[i], N, 1L);
    }
    for (long int i = 0L; i < M; ++i)
    {
      std :: fill_n(m_inT[i], K, 1L);
      std :: fill_n(m_noT[i], K, 1L);
      for (long int j = 0L; j < K; ++j)
      {
        weightsT[i][j] = new MagT64[N];
        std :: fill_n(weightsT[i][j], N, 1L);
      }
    }
    double * inputsT  = new double[N];
    std :: fill_n(inputsT, N, 1L);
    std :: unique_ptr < MagT64[] > hT;
    hT.reset(new MagT64[N]);
    MagT64 old_m_onT = mag :: bar(m_inT[a][k], m_noT[a][k]);
    for (long int i = 0L; i < N; ++i)
    {
      hT[i] = mag :: bar(m_j_starT[k][i], weightsT[a][k][i]);
    }
    double maxdiffT = theta_node_update_exact(m_j_starT[k], m_inT[a][k], inputsT, weightsT[a][k], m_noT[a][k], paramsT, N, N);

    // This must be always true since I am changing only the messages from the factor node to the variables node
    MagT64 new_old_m_onT = mag :: bar(m_inT[a][k], m_noT[a][k]);
    REQUIRE ( isclose(old_m_onT.value(), new_old_m_onT.value()) );
    for (long int i = 0L; i < N; ++i)
    {
      MagT64 new_hT = mag :: bar(m_j_starT[k][i], weightsT[a][k][i]);
      REQUIRE ( isclose(hT[i].value(), new_hT.value()) );
    }

    // Beforehand they are all 1s, afterwards they are all 0s but thanks to the dumping function they are actually DAMPING*1
    REQUIRE ( isclose(m_inT[a][k].mag, test_valueT) );
    REQUIRE ( isclose(m_noT[a][k].mag, test_valueT) );
    for (long int i = 0L; i < N; ++i)
    {
      REQUIRE ( isclose(m_j_starT[k][i].mag, test_valueT) );
      REQUIRE ( isclose(weightsT[a][k][i].mag, test_valueT) );
    }

    // At the beginning they are all ones and afterwards, but before damping, 0s
    REQUIRE ( isclose(maxdiffT, std :: tanh(1.)) );

      /////////////////
     // Second test //
   //////////////////
   // MagP64

   for (long int i = 0L; i < K; ++i)
   {
     std :: fill_n(m_j_starP[i], N, 1L);
   }
   for (long int i = 0L; i < M; ++i)
   {
     std :: fill_n(m_inP[i], K, 1L);
     std :: fill_n(m_noP[i], K, 0L);
     for (long int j = 0L; j < K; ++j)
     {
       std :: fill_n(weightsP[i][j], N, 0L);
     }
   }
   old_m_onP = mag :: bar(m_inP[a][k], m_noP[a][k]);
   for (long int i = 0L; i < N; ++i)
   {
     hP[i] = mag :: bar(m_j_starP[k][i], weightsP[a][k][i]);
   }
   maxdiffP = theta_node_update_exact(m_j_starP[k], m_inP[a][k], inputsP, weightsP[a][k], m_noP[a][k], paramsP, N, N);

   // This must be always true since I am changing only the messages from the factor node to the variables node
   new_old_m_onP = mag :: bar(m_inP[a][k], m_noP[a][k]);
   REQUIRE ( isclose(old_m_onP.mag, new_old_m_onP.mag) );
   for (long int i = 0L; i < N; ++i)
   {
     MagP64 new_hP = mag :: bar(m_j_starP[k][i], weightsP[a][k][i]);
     REQUIRE ( isclose(hP[i].mag, new_hP.mag) );
   }

   // Beforehand they are all 1s, afterwards they are all 0s but thanks to the dumping function they are actually (1-DAMPING)*1
   REQUIRE ( isclose(m_inP[a][k].mag, 1.) );
   REQUIRE ( isclose(m_noP[a][k].mag, revDAMPING) );
   for (long int i = 0L; i < N; ++i)
   {
     REQUIRE ( isclose(m_j_starP[k][i].mag, 1.) );
     REQUIRE ( isclose(weightsP[a][k][i].mag, 0.) );
   }

   // At the beginning they are all ones and afterwards, but before damping, 0s
   REQUIRE ( isclose(maxdiffP, 0.) );


   // MagT64

   for (long int i = 0L; i < K; ++i)
   {
     std :: fill_n(m_j_starT[i], N, 1L);
   }
   for (long int i = 0L; i < M; ++i)
   {
     std :: fill_n(m_inT[i], K, 1L);
     std :: fill_n(m_noT[i], K, 0L);
     for (long int j = 0L; j < K; ++j)
     {
       std :: fill_n(weightsT[i][j], N, 0L);
     }
   }
   hT.reset(new MagT64[N]);
   old_m_onT = mag :: bar(m_inT[a][k], m_noT[a][k]);
   for (long int i = 0L; i < N; ++i)
   {
     hT[i] = mag :: bar(m_j_starT[k][i], weightsT[a][k][i]);
   }
   maxdiffT = theta_node_update_exact(m_j_starT[k], m_inT[a][k], inputsT, weightsT[a][k], m_noT[a][k], paramsT, N, N);

   // This must be always true since I am changing only the messages from the factor node to the variables node
   new_old_m_onT = mag :: bar(m_inT[a][k], m_noT[a][k]);
   REQUIRE ( isclose(old_m_onT.value(), new_old_m_onT.value()) );
   for (long int i = 0L; i < N; ++i)
   {
     MagT64 new_hT = mag :: bar(m_j_starT[k][i], weightsT[a][k][i]);
     REQUIRE ( isclose(hT[i].value(), new_hT.value()) );
   }

   // Beforehand they are all 1s, afterwards they are all 0s but thanks to the dumping function they are actually DAMPING*1
   REQUIRE ( isclose(m_inT[a][k].mag, 1. + test2_valueT) );
   REQUIRE ( isclose(m_noT[a][k].mag, test2_valueT) );
   for (long int i = 0L; i < N; ++i)
   {
     REQUIRE ( isclose(m_j_starT[k][i].mag, 1.) );
     REQUIRE ( isclose(weightsT[a][k][i].mag, 0.) );
   }

   // At the beginning they are all ones and afterwards, but before damping, 0s
   REQUIRE ( isclose(maxdiffT, 0.) );

  }


  SECTION ( "TEST theta_node_update_accurate" )
  {

    // MagP64
    std :: string accuracy = "accurate";
    Params < MagP64 > paramsP(MAX_ITERS, DAMPING, EPSIL, static_cast < double >(NAN), 0., 0., accuracy, accuracy);

    for (long int i = 0L; i < K; ++i)
    {
      std :: fill_n(m_j_starP[i], N, 1L);
    }
    for (long int i = 0L; i < M; ++i)
    {
      std :: fill_n(m_inP[i], K, 1L);
      std :: fill_n(m_noP[i], K, 1L);
      for (long int j = 0L; j < K; ++j)
      {
        weightsP[i][j] = new MagP64[N];
        std :: fill_n(weightsP[i][j], N, 1L);
      }
    }
    double * inputsP  = new double[N];
    std :: fill_n(inputsP, N, 1L);
    std :: unique_ptr < MagP64[] > hP;
    hP.reset(new MagP64[N]);
    MagP64 old_m_onP = mag :: bar(m_inP[a][k], m_noP[a][k]);
    for (long int i = 0L; i < N; ++i)
    {
      hP[i] = mag :: bar(m_j_starP[k][i], weightsP[a][k][i]);
    }
    double maxdiffP = theta_node_update_accurate(m_j_starP[k], m_inP[a][k], inputsP, weightsP[a][k], m_noP[a][k], paramsP, N, N);

    // This must be always true since I am changing only the messages from the factor node to the variables node
    MagP64 new_old_m_onP = mag :: bar(m_inP[a][k], m_noP[a][k]);
    REQUIRE ( isclose(old_m_onP.mag, new_old_m_onP.mag) );
    for (long int i = 0L; i < N; ++i)
    {
      MagP64 new_hP = mag :: bar(m_j_starP[k][i], weightsP[a][k][i]);
      REQUIRE ( isclose(hP[i].mag, new_hP.mag) );
    }

    // Beforehand they are all 1s, afterwards they are all 0s but thanks to the dumping function they are actually DAMPING*1

    REQUIRE ( isclose(m_inP[a][k].mag, DAMPING) );
    REQUIRE ( isclose(m_noP[a][k].mag, DAMPING) );
    for (long int i = 0L; i < N; ++i)
    {
      REQUIRE ( isclose(m_j_starP[k][i].mag, DAMPING) );
      REQUIRE ( isclose(weightsP[a][k][i].mag, DAMPING) );
    }

    // At the beginning m_noP are all 1s and afterwards, but before damping, 0s
    REQUIRE ( isclose(maxdiffP, 1.) );


    // MagT64
    Params < MagT64 > paramsT(MAX_ITERS, DAMPING, EPSIL, static_cast < double >(NAN), 0., 0., accuracy, accuracy);

    for (long int i = 0L; i < K; ++i)
    {
      std :: fill_n(m_j_starT[i], N, 1L);
    }
    for (long int i = 0L; i < M; ++i)
    {
      std :: fill_n(m_inT[i], K, 1L);
      std :: fill_n(m_noT[i], K, 1L);
      for (long int j = 0L; j < K; ++j)
      {
        weightsT[i][j] = new MagT64[N];
        std :: fill_n(weightsT[i][j], N, 1L);
      }
    }
    double * inputsT  = new double[N];
    std :: fill_n(inputsT, N, 1L);
    std :: unique_ptr < MagT64[] > hT;
    hT.reset(new MagT64[N]);
    MagT64 old_m_onT = mag :: bar(m_inT[a][k], m_noT[a][k]);
    for (long int i = 0L; i < N; ++i)
    {
      hT[i] = mag :: bar(m_j_starT[k][i], weightsT[a][k][i]);
    }
    double maxdiffT = theta_node_update_accurate(m_j_starT[k], m_inT[a][k], inputsT, weightsT[a][k], m_noT[a][k], paramsT, N, N);

    // This must be always true since I am changing only the messages from the factor node to the variables node
    MagT64 new_old_m_onT = mag :: bar(m_inT[a][k], m_noT[a][k]);
    REQUIRE ( isclose(old_m_onT.value(), new_old_m_onT.value()) );
    for (long int i = 0L; i < N; ++i)
    {
      MagT64 new_hT = mag :: bar(m_j_starT[k][i], weightsT[a][k][i]);
      REQUIRE ( isclose(hT[i].value(), new_hT.value()) );
    }

    // Beforehand they are all 1s, afterwards they are all 0s but thanks to the dumping function they are actually DAMPING*1
    REQUIRE ( isclose(m_inT[a][k].mag, test_valueT) );
    REQUIRE ( isclose(m_noT[a][k].mag, test_valueT) );
    for (long int i = 0L; i < N; ++i)
    {
      REQUIRE ( isclose(m_j_starT[k][i].mag, test_valueT) );
      REQUIRE ( isclose(weightsT[a][k][i].mag, test_valueT) );
    }

    // At the beginning they are all ones and afterwards, but before damping, 0s
    REQUIRE ( isclose(maxdiffT, std :: tanh(1.)) );

  }


  SECTION ( "TEST theta_node_update_approx" )
  {

    // MagP64
    std :: string accuracy = "none";
    Params < MagP64 > paramsP(MAX_ITERS, DAMPING, EPSIL, static_cast < double >(NAN), 0., 0., accuracy, accuracy);

    for (long int i = 0L; i < K; ++i)
    {
      std :: fill_n(m_j_starP[i], N, 1L);
    }
    for (long int i = 0L; i < M; ++i)
    {
      std :: fill_n(m_inP[i], K, 1L);
      std :: fill_n(m_noP[i], K, 1L);
      for (long int j = 0L; j < K; ++j)
      {
        weightsP[i][j] = new MagP64[N];
        std :: fill_n(weightsP[i][j], N, 1L);
      }
    }
    double * inputsP  = new double[N];
    std :: fill_n(inputsP, N, 1L);
    std :: unique_ptr < MagP64[] > hP;
    hP.reset(new MagP64[N]);
    MagP64 old_m_onP = mag :: bar(m_inP[a][k], m_noP[a][k]);
    for (long int i = 0L; i < N; ++i)
    {
      hP[i] = mag :: bar(m_j_starP[k][i], weightsP[a][k][i]);
    }
    double maxdiffP = theta_node_update_approx(m_j_starP[k], m_inP[a][k], inputsP, weightsP[a][k], m_noP[a][k], paramsP, N, N);

    // This must be always true since I am changing only the messages from the factor node to the variables node
    MagP64 new_old_m_onP = mag :: bar(m_inP[a][k], m_noP[a][k]);
    REQUIRE ( isclose(old_m_onP.mag, new_old_m_onP.mag) );
    for (long int i = 0L; i < N; ++i)
    {
      MagP64 new_hP = mag :: bar(m_j_starP[k][i], weightsP[a][k][i]);
      REQUIRE ( isclose(hP[i].mag, new_hP.mag) );
    }

    // Beforehand they are all 1s, afterwards they are all 0s but thanks to the dumping function they are actually DAMPING*1

    REQUIRE ( isclose(m_inP[a][k].mag, DAMPING) );
    REQUIRE ( isclose(m_noP[a][k].mag, DAMPING) );
    for (long int i = 0L; i < N; ++i)
    {
      REQUIRE ( isclose(m_j_starP[k][i].mag, DAMPING) );
      REQUIRE ( isclose(weightsP[a][k][i].mag, DAMPING) );
    }

    // At the beginning m_noP are all 1s and afterwards, but before damping, 0s
    REQUIRE ( isclose(maxdiffP, 1.) );


    // MagT64
    Params < MagT64 > paramsT(MAX_ITERS, DAMPING, EPSIL, static_cast < double >(NAN), 0., 0., accuracy, accuracy);

    for (long int i = 0L; i < K; ++i)
    {
      std :: fill_n(m_j_starT[i], N, 1L);
    }
    for (long int i = 0L; i < M; ++i)
    {
      std :: fill_n(m_inT[i], K, 1L);
      std :: fill_n(m_noT[i], K, 1L);
      for (long int j = 0L; j < K; ++j)
      {
        weightsT[i][j] = new MagT64[N];
        std :: fill_n(weightsT[i][j], N, 1L);
      }
    }
    double * inputsT  = new double[N];
    std :: fill_n(inputsT, N, 1L);
    std :: unique_ptr < MagT64[] > hT;
    hT.reset(new MagT64[N]);
    MagT64 old_m_onT = mag :: bar(m_inT[a][k], m_noT[a][k]);
    for (long int i = 0L; i < N; ++i)
    {
      hT[i] = mag :: bar(m_j_starT[k][i], weightsT[a][k][i]);
    }
    double maxdiffT = theta_node_update_approx(m_j_starT[k], m_inT[a][k], inputsT, weightsT[a][k], m_noT[a][k], paramsT, N, N);

    // This must be always true since I am changing only the messages from the factor node to the variables node
    MagT64 new_old_m_onT = mag :: bar(m_inT[a][k], m_noT[a][k]);
    REQUIRE ( isclose(old_m_onT.value(), new_old_m_onT.value()) );
    for (long int i = 0L; i < N; ++i)
    {
      MagT64 new_hT = mag :: bar(m_j_starT[k][i], weightsT[a][k][i]);
      REQUIRE ( isclose(hT[i].value(), new_hT.value()) );
    }

    // Beforehand they are all 1s, afterwards they are all 0s but thanks to the dumping function they are actually DAMPING*1
    REQUIRE ( isclose(m_inT[a][k].mag, test_valueT) );
    REQUIRE ( isclose(m_noT[a][k].mag, test_valueT) );
    for (long int i = 0L; i < N; ++i)
    {
      REQUIRE ( isclose(m_j_starT[k][i].mag, test_valueT) );
      REQUIRE ( isclose(weightsT[a][k][i].mag, test_valueT) );
    }

    // At the beginning they are all ones and afterwards, but before damping, 0s
    REQUIRE ( isclose(maxdiffT, std :: tanh(1.)) );

  }

  SECTION ("TEST focusingBP")
  {
    // All input vectors = 1s
    double ** data1 = new double *[M];
    std :: generate_n(data1, M, [&](){return new double[N];});
    for (int i = 0; i < M; ++i) std :: fill_n(data1[i], N, 1.);

    // All input vectors = -1s
    double ** data2 = new double *[M];
    std :: generate_n(data2, M, [&](){return new double[N];});
    for (int i = 0; i < M; ++i) std :: fill_n(data2[i], N, -1.);

    // All labels = 1s
    long int * label1 = new long int[M];
    std :: fill_n(label1, M, 1L);

    // All labels = -1s
    long int * label2 = new long int[M];
    std :: fill_n(label2, M, -1L);

    // + / +
    Patterns pattern1 (data1, label1, M, N);

    // + / -
    Patterns pattern2 (data1, label2, M, N);

    // - / +
    Patterns pattern3 (data2, label1, M, N);

    // - / -
    Patterns pattern4 (data2, label2, M, N);


    std :: string accuracy = "exact";
    long int ** weights = nullptr;
    std :: string fprotocol = "pseudo_reinforcement";
    FocusingProtocol fp(fprotocol, MAX_STEPS);
    double result;

    std :: mt19937 engine(SEED);
    std :: uniform_int_distribution < int > dist(1, 99);

    for (int j=0; j<10; ++j)
    {
      int seed = dist(engine);
      weights = focusingBP < MagP64 > (K, pattern1, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result > 0 );

      weights = focusingBP < MagP64 > (K, pattern2, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result < 0 );

      weights = focusingBP < MagP64 > (K, pattern3, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result < 0 );

      weights = focusingBP < MagP64 > (K, pattern4, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result > 0 );


      weights = focusingBP < MagT64 > (K, pattern1, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result > 0 );

      weights = focusingBP < MagT64 > (K, pattern2, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result < 0 );

      weights = focusingBP < MagT64 > (K, pattern3, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result < 0 );

      weights = focusingBP < MagT64 > (K, pattern4, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result > 0 );



      accuracy = "accurate";

      weights = focusingBP < MagP64 > (K, pattern1, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result > 0 );

      weights = focusingBP < MagP64 > (K, pattern2, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result < 0 );

      weights = focusingBP < MagP64 > (K, pattern3, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result < 0 );

      weights = focusingBP < MagP64 > (K, pattern4, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result > 0 );


      weights = focusingBP < MagT64 > (K, pattern1, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result > 0 );

      weights = focusingBP < MagT64 > (K, pattern2, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result < 0 );

      weights = focusingBP < MagT64 > (K, pattern3, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result < 0 );

      weights = focusingBP < MagT64 > (K, pattern4, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result > 0 );


      accuracy = "none";

      weights = focusingBP < MagP64 > (K, pattern1, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result > 0 );

      weights = focusingBP < MagP64 > (K, pattern2, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result < 0 );

      weights = focusingBP < MagP64 > (K, pattern3, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result < 0 );

      weights = focusingBP < MagP64 > (K, pattern4, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result > 0 );


      weights = focusingBP < MagT64 > (K, pattern1, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result > 0 );

      weights = focusingBP < MagT64 > (K, pattern2, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result < 0 );

      weights = focusingBP < MagT64 > (K, pattern3, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result < 0 );

      weights = focusingBP < MagT64 > (K, pattern4, MAX_ITERS, MAX_STEPS, seed, DAMPING, accuracy, accuracy, RANDFACT, fp, EPSIL, 1, "", "");
      result = std :: accumulate( weights, weights + K, 0., [](const double & val, const long int * wk) {return val + std :: accumulate(wk, wk + M, 0.);} );
      REQUIRE ( result > 0 );
    }



  }


}
