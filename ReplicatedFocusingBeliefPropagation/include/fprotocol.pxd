# distutils: language = c++
# cython: language_level=2

from libcpp.string cimport string
from libcpp.memory cimport unique_ptr

cdef extern from "fprotocol.h":
  cppclass FocusingProtocol:
    FocusingProtocol() except +
    FocusingProtocol(string, long int) except +

    # Members
    long int Nrep
    unique_ptr[double[]] gamma, n_rep, beta

    # Attributes
    void StandardReinforcement( const double *rho, const long int &Nrho)
    void StandardReinforcement( const double &drho)
    void Scoping( const double *gr, const double &x, const long int &ngr)
    void PseudoReinforcement( const double *rho, const long int &nrho, double x)
    void PseudoReinforcement( const double &drho, const double &x)
    void FreeScoping( double **list, const long int &nlist)
