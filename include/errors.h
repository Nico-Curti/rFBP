#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>
static constexpr int ERROR_POSITIVE = 1;
static constexpr int ERROR_RANGE    = 2;
static constexpr int ERROR_ACCURACY = 3;
static constexpr int ERROR_EXACT    = 4;
static constexpr int ERROR_INFINITE = 5;
static constexpr int ERROR_WEIGHTS  = 6;
static constexpr int ERROR_PATTERN  = 7;
static constexpr int ERROR_PROTOCOL = 8;


inline void error_Npositive(const long int &N)
{
  std::cerr << "N must be positive; given: " << N << std::endl;
  std::exit(ERROR_POSITIVE);
}

inline void error_Kpositive(const int &K)
{
  std::cerr << "K must be positive; given: " << K << std::endl;
  std::exit(ERROR_POSITIVE);
}

inline void error_maxiters(const long int &max_iters)
{
  std::cerr << "max_iters must be non-negative; given: " << max_iters << std::endl;
  std::exit(ERROR_POSITIVE);
}

inline void error_maxsteps(const long int &max_steps)
{
  std::cerr << "max_steps must be non-negative; given: " << max_steps << std::endl;
  std::exit(ERROR_POSITIVE);
}

inline void error_damping(const double &damping)
{
  std::cerr << "damping must be in [0,1); given: " << damping << std::endl;
  std::exit(ERROR_RANGE);
}

inline void error_randfact(const double &randfact)
{
  std::cerr << "randfact must be in [0,1); given: " << randfact << std::endl;
  std::exit(ERROR_RANGE);
}

inline void error_accuracy1(const std::string &accuracy1)
{
  std::cerr << "accuracy1 must be one of 'exact', 'accurate' or 'none'; given: " << accuracy1 << std::endl;
  std::exit(ERROR_ACCURACY);
}

inline void error_accuracy2(const std::string &accuracy2)
{
  std::cerr << "accuracy2 must be one of 'exact', 'accurate' or 'none'; given: " << accuracy2 << std::endl;
  std::exit(ERROR_ACCURACY);
}

inline void error_Nexact(const int &N)
{
  std::cerr << "when accuracy1 == 'exact', N must be odd; given: " << N << std::endl;
  std::exit(ERROR_EXACT);
}

inline void error_Kexact(const int &K)
{
  std::cerr << "when accuracy2 == 'exact', K must be odd; given: " << K << std::endl;
  std::exit(ERROR_EXACT);
}

inline void error_infinite(const double &beta)
{
#ifdef _OPENMP
#pragma omp single
#endif
  std::cerr << "finite beta not yet supported; given: " << beta << std::endl;
  std::exit(ERROR_INFINITE);
}

inline void error_message_weights(const std::string &filename)
{
#ifdef _OPENMP
#pragma omp single
#endif
  std::cerr << "Weights file not found! Given : " << filename << std::endl;
  std::exit(ERROR_WEIGHTS);
}

inline void error_pattern(const std::string &filename)
{
  std::cerr << "Input file not found! Given: " << filename << std::endl;
  std::exit(ERROR_PATTERN);
}

inline void error_protocol(const std::string &prot)
{
  std::cerr << "Invalid protocol parameter. Given: " << prot
            << ". Possible values are \"scoping\", \"pseudo_reinforcement\", \"free_scoping\" and \"standard_reinforcement\""
            << std::endl;
  std::exit(ERROR_PATTERN);
}

#endif // ERRORS_H
