#ifndef __errors_h__
#define __errors_h__

#include <iostream>
#include <string>

static constexpr int ERROR_POSITIVE         = 1;  ///< Return Error
static constexpr int ERROR_RANGE            = 2;  ///< Return Error
static constexpr int ERROR_ACCURACY         = 3;  ///< Return Error
static constexpr int ERROR_EXACT            = 4;  ///< Return Error
static constexpr int ERROR_INFINITE         = 5;  ///< Return Error
static constexpr int ERROR_WEIGHTS          = 6;  ///< Return Error
static constexpr int ERROR_PATTERN          = 7;  ///< Return Error
static constexpr int ERROR_PROTOCOL         = 8;  ///< Return Error
static constexpr int ERROR_MESSAGES         = 91; ///< Return Error
static constexpr int ERROR_INVALID_MESSAGES = 92; ///< Return Error
// ERRORS 101-105 are related to raising errors in parse_args.h
static constexpr int ERROR_MAGNETIZATION    = 111; ///< Return Error
static constexpr int ERROR_ATANHERF_FILE    = 112; ///< Return Error

/**
* @brief raise error because N is not positive
*
* @param N the given value of N
*/
inline void error_Npositive (const long int & N)
{
  std :: cerr << "N must be positive; given: " << N << std :: endl;
  std :: exit(ERROR_POSITIVE);
}

/**
* @brief raise error because K is not positive
*
* @param K the given value of K
*/
inline void error_Kpositive (const int & K)
{
  std :: cerr << "K must be positive; given: " << K << std :: endl;
  std :: exit(ERROR_POSITIVE);
}

/**
* @brief raise error because max_iters is not positive
*
* @param max_iters the given value of max_iters
*/
inline void error_maxiters (const long int & max_iters)
{
  std :: cerr << "max_iters must be non-negative; given: " << max_iters << std :: endl;
  std :: exit(ERROR_POSITIVE);
}

/**
* @brief raise error because max_steps is not positive
*
* @param max_steps the given value of max_stes
*/
inline void error_maxsteps (const long int & max_steps)
{
  std :: cerr << "max_steps must be non-negative; given: " << max_steps << std :: endl;
  std :: exit(ERROR_POSITIVE);
}

/**
* @brief raise error because damping is not bounded in [0, 1)
*
* @param damping the given value of damping
*/
inline void error_damping (const double & damping)
{
  std :: cerr << "damping must be in [0,1); given: " << damping << std :: endl;
  std :: exit(ERROR_RANGE);
}

/**
* @brief raise error because randfact is not bounded in [0, 1)
*
* @param randfact the given value of randfact
*/
inline void error_randfact (const double & randfact)
{
  std :: cerr << "randfact must be in [0,1); given: " << randfact << std :: endl;
  std :: exit(ERROR_RANGE);
}

/**
* @brief raise error because accuracy is not valid
*
* @param accuracy the given value of accuracy
*/
inline void error_accuracy (const std :: string & accuracy)
{
  std :: cerr << "Invalid accuracy variable given. Given : " << accuracy << std :: endl;
  std :: exit(ERROR_ACCURACY);
}

/**
* @brief raise error because too many accuracies were given
*
*/
inline void error_num_accuracy ()
{
  std :: cerr << "Too many accuracy variables given. Needed two." << std :: endl;
  std :: exit(ERROR_ACCURACY);
}

/**
* @brief raise error because accuracy1 is not valid
*
* @param accuracy1 the given value of accuracy1
*/
inline void error_accuracy1 (const std :: string & accuracy1)
{
  std :: cerr << "accuracy1 must be one of 'exact', 'accurate' or 'none'; given: " << accuracy1 << std :: endl;
  std :: exit(ERROR_ACCURACY);
}

/**
* @brief raise error because accuracy2 is not valid
*
* @param accuracy2 the given value of accuracy2
*/
inline void error_accuracy2 (const std :: string & accuracy2)
{
  std :: cerr << "accuracy2 must be one of 'exact', 'accurate' or 'none'; given: " << accuracy2 << std :: endl;
  std :: exit(ERROR_ACCURACY);
}

/**
* @brief raise error because with accuracy1 == 'exact' the value of N must be odd
*
* @param N the given value of N
*/
inline void error_Nexact (const int & N)
{
  std :: cerr << "when accuracy1 == 'exact', N must be odd; given: " << N << std :: endl;
  std :: exit(ERROR_EXACT);
}

/**
* @brief raise error because with accuracy2 == 'exact' the value of K must be odd
*
* @param K the given value of K
*/
inline void error_Kexact (const int & K)
{
  std :: cerr << "when accuracy2 == 'exact', K must be odd; given: " << K << std :: endl;
  std :: exit(ERROR_EXACT);
}

/**
* @brief raise error because beta is not finite (not yet supported)
*
* @param beta the given value of beta
*/
inline void error_infinite (const double & beta)
{
#ifdef _OPENMP
#pragma omp single
#endif
  std :: cerr << "finite beta not yet supported; given: " << beta << std :: endl;
  std :: exit(ERROR_INFINITE);
}

/**
* @brief raise error because weight filename is not found
*
* @param filename the given value of filename
*/
inline void error_message_weights (const std :: string & filename)
{
#ifdef _OPENMP
#pragma omp single
#endif
  std :: cerr << "Weights file not found! Given : " << filename << std :: endl;
  std :: exit(ERROR_WEIGHTS);
}

/**
* @brief raise error because input filename is not found
*
* @param filename the given value of filename
*/
inline void error_pattern (const std :: string & filename)
{
  std :: cerr << "Input file not found! Given: " << filename << std :: endl;
  std :: exit(ERROR_PATTERN);
}

/**
* @brief raise error because protocol type is invalid
*
* @param prot the given value of protocol type
*/
inline void error_protocol (const std :: string & prot)
{
  std :: cerr << "Invalid protocol parameter. Given: " << prot
              << ". Possible values are \"scoping\", \"pseudo_reinforcement\", \"free_scoping\" and \"standard_reinforcement\""
              << std :: endl;
  std :: exit(ERROR_PATTERN);
}

/**
* @brief raise error because messages filename is not found
*
* @param filename the given value of filename
*/
inline void error_messages (const std :: string & filename)
{
  std :: cerr << "Messages file not found! Given: " << filename << std :: endl;
  std :: exit(ERROR_MESSAGES);
}

/**
* @brief raise error because messages filename is not well formatted
*
* @param filename the given value of filename
*/
inline void error_invalid_messages (const std :: string & filename)
{
  std :: cerr << "Invalid messages file! Given: " << filename << std :: endl;
  std :: exit(ERROR_INVALID_MESSAGES);
}

/**
* @brief raise error because Mag parameter is not valid
*
* @param mag the given value of mag
*/
inline void error_magnetization (const int & mag)
{
  std :: cerr << "Invalid magnetization found. Given : " << mag << std :: endl;
  std :: exit(ERROR_MAGNETIZATION);
}

/**
* @brief raise error because atanherf interpolation filename is not found.
* Suggest how to download it.
*
*/
inline void error_atanherf_file ()
{
  std :: cerr << "Atanherf interpolation coefficients file not found."
                 " Please download it before running to prevent some troubles."
                 " The appropriated script could be used and you can find it in the 'scripts' directory."
                 " Just run 'python download_atanherf.py'"
              << std :: endl;
  std :: exit(ERROR_ATANHERF_FILE);
}

#endif // __errors_h__
