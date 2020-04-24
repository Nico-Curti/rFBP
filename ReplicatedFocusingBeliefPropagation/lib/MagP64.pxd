# distutils: language = c++
# cython: language_level=2

cdef extern from "magP.h":

  cppclass MagP64:

    MagP64 () except +
    MagP64 (const double & x) except +
