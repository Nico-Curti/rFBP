# distutils: language = c++
# cython: language_level=2

from libcpp.string cimport string
from libcpp.memory cimport unique_ptr

cdef extern from "fprotocol.h":

  cppclass FocusingProtocol:

    # Constructors
    FocusingProtocol () except +
    FocusingProtocol (string protocol, long int size) except +

    # Members
    long int Nrep
    unique_ptr[double[]] gamma
    unique_ptr[double[]] n_rep
    unique_ptr[double[]] beta

    # Attributes
    void StandardReinforcement (const double * rho, const long int & Nrho)
    void StandardReinforcement (const double & drho)
    void Scoping (const double * gr, const double & x, const long int & ngr)
    void PseudoReinforcement (const double * rho, const long int & nrho, double x)
    void PseudoReinforcement (const double & drho, const double & x)
    void FreeScoping (double ** list, const long int & nlist)
