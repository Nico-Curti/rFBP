# distutils: language = c++
# cython: language_level=2

from libcpp cimport bool
from libcpp.string cimport string
from libcpp.memory cimport unique_ptr

cdef extern from "pattern.h":

  cppclass Patterns:

    # Constructors
    Patterns () except +
    Patterns (const string & filename, bool bin, const string & delimiter) except +
    Patterns (const long int & N, const long int & M) except +
    Patterns (double ** data, long int * label, const int & Nrow, const int & Ncol) except +

    # Attributes
    long int Nrow
    long int Ncol
    long int Nout
    long int * output
    double ** input


cdef class _Patterns:
  cdef unique_ptr[Patterns] thisptr

  cdef public:
    long int Nrow
    long int Ncol
