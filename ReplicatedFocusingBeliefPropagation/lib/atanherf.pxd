# distutils: language = c++
# cython: language_level=2

cdef extern from "atanherf.h" namespace "AtanhErf":

  double atanherf_largex (const double & x);
  double atanherf_interp (const double & x);
  double evalpoly (const double & x);
  double atanherf (const double & x);
