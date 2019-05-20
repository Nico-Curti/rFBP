# distutils: language = c++
# cython: language_level=2

from libcpp.string cimport string
from libcpp cimport bool

cdef extern from "pattern.h":
  cppclass Patterns:
    Patterns() except +
    Patterns(const string &, bool, const string &) except +
    Patterns(const long int &, const long int &) except +
    Patterns(double **, long int *, const int &, const int &) except +

    # Attributes
    long int Nrow, Ncol, Nout
    long int* output
    double ** input
