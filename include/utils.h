#ifndef __utils_h__
#define __utils_h__

#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>
#include <memory>
#include <cmath>
//#if __has_include(<filesystem>)
//
//#  include <filesystem>
//#  define have_filesystem 1
//
//#endif
#ifdef _OPENMP

  #include <omp.h>

#else

  #include <chrono>

#endif

static constexpr double INF     = std :: numeric_limits < double > :: infinity(); ///< shortcut to infinity value
static constexpr double epsilon = std :: numeric_limits < double > :: epsilon(); ///< shortcut to epsilon value

#if (defined(__GNUC__) && !defined(__clang__))

  static constexpr double log_2       = std :: log(2);      ///< shortcut to log(2)
  static constexpr double log2_over_2 = std :: log(2) * .5; ///< shortcut to log(2)/2

#else

  static constexpr double log_2       = 0.6931471805599453;     ///< shortcut to log(2)
  static constexpr double log2_over_2 = 0.34657359027997264311; ///< shortcut to log(2)/2

#endif

/**
* @brief sign function
*
* @param x value
*
*/
double sign (const double & x);

enum {scoping_ = 0L,
      pseudo_,
      free_scoping_,
      standard_
      };

enum {magP = 0L,
      magT = 1
      };

static std :: unordered_map < std :: string, long int > protocol
{
  {"scoping",                scoping_},
  {"pseudo_reinforcement",   pseudo_},
  {"free_scoping",           free_scoping_},
  {"standard_reinforcement", standard_}
};

/**
* @brief split string to token
*
* @param txt input string
* @param del delimiter as string
*
* @returns std::vector<std::string> vector of token
*
*/
std :: vector < std :: string > split (const std :: string & txt, const std :: string & del);

#if __cplusplus < 201400

namespace std
{

/**
* @brief wrap for the older version of gcc anc clang
*
* @tparam T type of the pointer array
*
* @param size lenght of unique_ptr array
*
* @returns pointer array as unique_ptr (e.g. std :: unique_ptr < float[] > in modern c++)
*
*/
template < typename T >
std :: unique_ptr < T > make_unique ( std :: size_t size );

}

#endif

/**
* @brief check if a given file exists
*
* @param filename filename/path to check
*
* @returns bool true if the filename is found else false
*
*/
bool file_exists (const std :: string & filename);

/**
* @brief Evaluate current time
*
* @details this function use OpenMP API if the code is compiled with -fopenmp else it uses chrono functions
*
* @returns current time as double (OpenMP) or chrono
*
*/
#ifdef _OPENMP

  double what_time_is_it_now ();

#else

  std :: chrono :: time_point < std :: chrono :: high_resolution_clock > what_time_is_it_now ();

#endif

/**
* @brief Evaluate duration time
*
* @details this function is used in combination with what_time_is_it_now
*
* @tparam Time the type returned by what_time_is_it_now function
*
* @param start time evaluated by what_time_is_it_now function
*
* @returns evaluated time
*
*/
template < typename Time >
auto duration (const Time & start);


#endif // __utils_h__
