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
  * in `$PWD/data/atanherf_interp.max_16.step_0.0001.first_1.dat`.
  *
  * The variable PWD is defined at compile time and its value is set by the CMake file.
  * If you want to use a file in a different location, please re-build the library setting
  * the variable
  *
  * \code{.sh}
  * -DPWD='new/path/location'
  * \endcode
  *
  * @return Spline object with the interpolated coordinates.
  */
  spline getinp ();

  /**
  * @brief Atanh of erf function for large values of x.
  *
  * @param x Input variable.
  *
  * @return Approximated result of atanherf function.
  */
  double atanherf_largex (const double & x);

  /**
  * @brief Atanh of erf function computed with the interpolation coordinates
  * extracted by the spline.
  *
  * @param x Input variable.
  *
  * @return Approximated result of atanherf function estimated using a pre-computed LUT. The
  * LUT is generated using a cubic spline interpolation.
  */
  double atanherf_interp (const double & x);

  /**
  * @brief Atanh of erf function evaluated as polynomial decomposition.
  *
  * @param x Value as argument of atanherf function.
  *
  * @return Approximated result of atanherf function.
  */
  double evalpoly (const double & x);

  /**
  * @brief Atanh of erf function.
  *
  * @details The result is evaluated with different numerical techniques according to its domain.
  *
  * In particular:
  * - if its abs is lower than 2    -> "standard" formula
  * - if its abs is lower than 15   -> `atanherf_interp` formula
  * - if its abs is greater than 15 -> `atanherf_largex` formula
  *
  * @param x Input variable.
  *
  * @return Approximated result of atanherf function.
  *
  * @note The function automatically use the most appropriated approximation of the atanherf function
  * to prevent possible overflows.
  */
  double atanherf (const double & x);
}

#endif // __atanherf_h__
