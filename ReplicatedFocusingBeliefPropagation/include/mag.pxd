# distutils: language = c++
# cython: language_level=2

cdef extern from "magP.h":
  cppclass MagP64:
    MagP64() except +
    MagP64(const double &x) except +

cdef extern from "magT.h":
  cppclass MagT64:
    MagT64() except +
    MagT64(const double &x, double m) except +


ctypedef fused mag_t:
  MagP64
  MagT64
