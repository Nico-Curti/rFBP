#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <scorer.h>
#if __has_include(<filesystem>)
#  include <filesystem>
#  define have_filesystem 1
#endif
#ifdef _OPENMP
#include <omp.h>
#else
#include <chrono>
#endif

// static constexpr double inf         = std::numeric_limits<double>::infinity();
static constexpr double epsilon     = std :: numeric_limits < double > :: epsilon();

#if (defined(__GNUC__) && !defined(__clang__))
  static constexpr double log_2       = std :: log(2);
  static constexpr double log2_over_2 = std :: log(2) * .5;
#else
  static constexpr double log_2       = 0.6931471805599453;
  static constexpr double log2_over_2 = 0.34657359027997264311;
#endif


static inline auto sign (const double & x){ return (x > 0.) ? 1. : (x < 0. ? -1. : 0); }


template < class Mag >  using MagVec  = Mag *;
template < class Mag >  using MagVec2 = MagVec < Mag > *;
template < class Mag >  using MagVec3 = MagVec2 < Mag > *;

enum {scoping_ = 0L,
      pseudo_,
      free_scoping_,
      standard_
      };

enum {magP = 0,
      magT = 1
      };

static std :: unordered_map < std :: string, long int > protocol
{
  {"scoping",                scoping_},
  {"pseudo_reinforcement",   pseudo_},
  {"free_scoping",           free_scoping_},
  {"standard_reinforcement", standard_}
};

std :: vector < std :: string > split (const std :: string & txt, const std :: string & del);

inline bool file_exists (const std :: string & filename)
{
#if have_filesystem == 1
  return std :: filesystem :: exists(std :: filesystem :: path(filename));
#else
  if (FILE *file = fopen(filename.c_str(), "r"))
  {
    fclose(file);
    return true;
  }
  return false;
#endif
}

inline const auto what_time_is_it_now()
{
#ifdef _OPENMP
  return omp_get_wtime();
#else
  return std :: chrono :: high_resolution_clock :: now();
#endif
}

template < typename Time > inline const auto duration (const Time & start)
{
#ifdef _OPENMP
  return omp_get_wtime() - start;
#else
  return std :: chrono :: duration_cast < std :: chrono :: seconds >(std :: chrono :: high_resolution_clock :: now() - start).count();
#endif
}

//static void menu_FBP()
//{
//    std :: cout << std :: endl;
//    std :: cout << " BinaryCommitteeMachineFBP Algorithm"  << std :: endl;
//    std :: cout << " ====================================" << std :: endl;
//    std :: cout << " *                                  *" << std :: endl;
//    std :: cout << " *       University of Bologna      *" << std :: endl;
//    std :: cout << " *                                  *" << std :: endl;
//    std :: cout << " *            Nico Curti            *" << std :: endl;
//    std :: cout << " *                &                 *" << std :: endl;
//    std :: cout << " *         Daniele Dall'Olio        *" << std :: endl;
//    std :: cout << " *                                  *" << std :: endl;
//    std :: cout << " *       nico.curti2@unibo.it       *" << std :: endl;
//    std :: cout << " * daniele.dallolio@studio.unibo.it *" << std :: endl;
//    std :: cout << " *                                  *" << std :: endl;
//    std :: cout << " ====================================" << std :: endl;
//    std :: cout << "          Optimized Library          " << std :: endl << std :: endl;
//}

#endif // UTILS_H
