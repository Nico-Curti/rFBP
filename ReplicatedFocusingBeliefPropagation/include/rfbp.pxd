# distutils: language = c++
# cython: language_level=2

from libcpp.string cimport string
from pattern cimport Patterns
from fprotocol cimport FocusingProtocol
#from libcpp cimport bool

cdef extern from "rfbp.h":
  cdef long int ** focusingBP[Mag](const long int &K,
                                   const Patterns &patterns,
                                   const long int &max_iters,
                                   const long int &max_steps,
                                   const long int &seed,
                                   const double &damping,
                                   const string &accuracy1,
                                   const string &accuracy2,
                                   const double &randfact,
                                   const FocusingProtocol &fprotocol,
                                   const double &epsil
                                   #string outfile=*,
                                   #string outmessfiletmpl=*,
                                   #string initmess=*,
                                   #const bool &bin_mess=*
                                   )

  cdef long int* nonbayes_test(long int ** const, const Patterns &, const long int &)

