#ifndef ATANHERF_H
#define ATANHERF_H

#ifndef PWD
#define PWD '.'
#endif

#include <cmath>
#include <fstream>
#include <algorithm>
#include <memory>
#include <numeric>
#include <iostream>

#include <utils.h>
#include <spline.h>

namespace AtanhErf
{
  spline getinp();
  double atanherf_largex(const double & x);
  double atanherf_interp(const double & x);
  double evalpoly(const double & x);
  double atanherf(const double & x);
}

#endif // ATANHERF_H
