# distutils: language = c++
# cython: language_level=2

cdef extern from "magT.h" nogil:

  cppclass MagT64:

    MagT64 () except +
    MagT64 (const double & x, double m) except +

