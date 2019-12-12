#ifndef __atanherf_h__
#define __atanherf_h__

#ifndef PWD
#define PWD '.'
#endif

#include <cmath>
#include <fstream>
#include <algorithm>
#include <memory>
#include <numeric>
#include <iostream>

#include <utils.hpp>
#include <spline.h>

namespace AtanhErf
{
  /**
  * @brief Load spline data points from file.
  *
  * @details The filename is hard coded into the function body and it must be placed
  * in "$PWD/data/atanherf_interp.max_16.step_0.0001.first_1.dat".
  * The variable PWD is defined at compile time and its value is set by the CMake file
  *
  * @return spline object with the interpolated coordinates
  */
  spline getinp ();

  /**
  * @brief Atanh of erf function for large values of x
  *
  * @param x double value as argument of atanherf function
  *
  * @return double approximated result of atanherf function
  */
  double atanherf_largex (const double & x);

  /**
  * @brief Atanh of erf function computed with the interpolation coordinates
  * extracted by the spline
  *
  * @param x double value as argument of atanherf function
  *
  * @return double approximated result of atanherf function
  */
  double atanherf_interp (const double & x);

  /**
  * @brief Atanh of erf function evaluated as polynomial decomposition
  *
  * @param x double value as argument of atanherf function
  *
  * @return double approximated result of atanherf function
  */
  double evalpoly (const double & x);

  /**
  * @brief Atanh of erf function
  * The result is evaluated with different numerical techniques according to
  * its domain. In particular:
  * - if its abs is lower than 2  => standard formula
  * - if its abs is lower than 15 => atanherf_interp formula
  * - if its abs is greater than 15 => atanherf_largex formula
  *
  * @param x double value as argument of atanherf function
  *
  * @return double approximated result of atanherf function
  */
  double atanherf (const double & x);
}

#endif // __atanherf_h__
