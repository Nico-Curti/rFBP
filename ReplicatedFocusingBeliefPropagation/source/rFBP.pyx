# distutils: language = c++
# cython: language_level=2

from libcpp.memory cimport unique_ptr
from cython.operator cimport dereference as deref
cimport numpy as np

from rfbp cimport focusingBP
from rfbp cimport nonbayes_test
from pattern cimport Patterns
from fprotocol cimport FocusingProtocol
from mag cimport MagP64
from mag cimport MagT64
from misc cimport double_pointers_for_cython
from misc cimport unique_pointer_to_pointer

from sklearn.utils import check_X_y
from sklearn.utils import check_array

from enum import Enum

class Mag (Enum):
  magP = 0
  magT = 1


cdef class _FocusingProtocol:
  cdef unique_ptr[FocusingProtocol] thisptr

  cdef public:
    long int Nrep

  def __init__(self, protocol, size):

    self.thisptr.reset(new FocusingProtocol(protocol, size))

    self.Nrep = deref(self.thisptr).Nrep

  def StandardReinforcement (self, double[::1] rho):
    cdef int size = len(rho)
    deref(self.thisptr).StandardReinforcement(&rho[0], size)
    return self

  def Scoping (self, double[::1] gr, x):
    cdef int size = len(gr)
    deref(self.thisptr).Scoping(&gr[0], x, size)
    return self

  def PseudoReinforcement (self, double[::1] rho, x):
    cdef int size = len(rho)
    deref(self.thisptr).PseudoReinforcement(&rho[0], size, x)
    return self

  def FreeScoping (self, arr):

    cdef np.ndarray[double, ndim=2, mode='c'] cpp_arr = arr
    cdef int size = len(arr)
    deref(self.thisptr).FreeScoping(double_pointers_for_cython[double, double](&cpp_arr[0, 0], size, 3), size)
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





cdef class _Pattern:
  cdef unique_ptr[Patterns] thisptr

  cdef public:
    long int Nrow
    long int Ncol

  def __init__ (self, double[::1] X=None, long int[::1] y=None, filename=b'', binary=False, delimiter=b'\t', M=-1, N=-1):

    cdef int cpp_M, cpp_N

    if X is not None and y is not None:

      # convert matrix to Pattern obj

      cpp_N, cpp_M = (M, N)

      self.thisptr.reset(new Patterns(double_pointers_for_cython[double, double](&X[0], N, M), &y[0], N, M))

    elif filename:

      # load Pattern from file

      self.thisptr.reset(new Patterns(filename, binary, delimiter))

    elif M > 0 and N > 0:

      # random pattern
      self.thisptr.reset(new Patterns(M, N))

    else:

      raise ValueError('Wrong input variable provided. Please give (X, y) or a valid filename')

    self.Nrow = deref(self.thisptr).Nrow
    self.Ncol = deref(self.thisptr).Ncol

  @property
  def labels (self):
    return [deref(self.thisptr).output[i] for i in range(self.Nrow)]

  @property
  def data (self):
    data = [[deref(self.thisptr).input[i][j] for j in range(self.Ncol)] for i in range(self.Nrow)]
    return data

  def __repr__ (self):
    class_name = self.__class__.__name__
    return '<{} Class (M: {} N: {} >)'.format(class_name, self.Nrow, self.Ncol)






def _rfbp (mag, _Pattern pattern, _FocusingProtocol protocol,
           hidden=3, max_iter=1000, max_steps=101, randfact=1e-1,
           damping=5e-1, epsil=1e-1, accuracy=(b'accurate', b'exact'),
           seed=135, nth=1):

  acc1, acc2 = accuracy

  cdef long int ** weights

  if mag == Mag.magP:
    weights = focusingBP[MagP64](hidden, deref(pattern.thisptr.get()), max_iter, max_steps, seed, damping, acc1, acc2, randfact, deref(protocol.thisptr.get()), epsil, nth)

  elif mag == Mag.magT:
    weights = focusingBP[MagT64](hidden, deref(pattern.thisptr.get()), max_iter, max_steps, seed, damping, acc1, acc2, randfact, deref(protocol.thisptr.get()), epsil, nth)

  else:
    raise TypeError('Invalid Magnetization given. Possible values are stored in Mag Enum {magP, magT}')

  return [[int(weights[i][j]) for j in range(pattern.Ncol)] for i in range(hidden)]


def _nonbayes_test (weights, _Pattern pattern, K):

  nlabel = pattern.Nrow
  weights = check_array(weights)

  cdef int row_size, column_size

  cdef np.ndarray['long int', ndim=2, mode='c'] weights_pointer = weights
  row_size, column_size = weights.shape

  cdef long int ** c_weights  = double_pointers_for_cython['long int', 'long int'](&weights_pointer[0, 0], row_size, column_size)
  cdef long int * predicted_labels = nonbayes_test(c_weights, pattern.thisptr.get()[0], K)

  return [int(predicted_labels[i]) for i in range(nlabel)]
