# distutils: language = c++
# cython: language_level=2

from libcpp.memory cimport unique_ptr
from libcpp.string cimport string
from cython.operator cimport dereference as deref

from FocusingProtocol cimport FocusingProtocol
from misc cimport double_pointers_for_cython
from misc cimport unique_pointer_to_pointer



cdef class _FocusingProtocol:

  def __init__(self, string protocol, int size):

    self.thisptr.reset(new FocusingProtocol(protocol, size))
    self.Nrep = deref(self.thisptr).Nrep

  def StandardReinforcement (self, double[::1] rho):
    cdef int size = len(rho)
    deref(self.thisptr).StandardReinforcement(&rho[0], size)
    return self

  def Scoping (self, double[::1] gr, double x):
    cdef int size = len(gr)
    deref(self.thisptr).Scoping(&gr[0], x, size)
    return self

  def PseudoReinforcement (self, double[::1] rho, double x):
    cdef int size = len(rho)
    deref(self.thisptr).PseudoReinforcement(&rho[0], size, x)
    return self

  def FreeScoping (self, double[::1] arr):

    cdef int size = len(arr)
    deref(self.thisptr).FreeScoping(double_pointers_for_cython[double, double](&arr[0], size, 3), size)
    return self

  @property
  def gamma (self):
    cdef double * gamma = unique_pointer_to_pointer(deref(self.thisptr).gamma, self.Nrep)
    return [gamma[i] for i in range(self.Nrep)]

  @property
  def n_rep (self):
    cdef double * n_rep = unique_pointer_to_pointer(deref(self.thisptr).n_rep, self.Nrep)
    return [n_rep[i] for i in range(self.Nrep)]

  @property
  def beta (self):
    cdef double * beta = unique_pointer_to_pointer(deref(self.thisptr).beta, self.Nrep)
    return [beta[i] for i in range(self.Nrep)]

  def __repr__ (self):
    class_name = self.__class__.__name__
    return '< {0} Cython Class (number of replicas: {1:d}) >'.format(class_name, self.Nrep)
