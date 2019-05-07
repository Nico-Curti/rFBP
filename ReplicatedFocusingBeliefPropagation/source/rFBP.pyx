# distutils: language = c++
# cython: language_level=2

cimport cython
from libcpp.memory cimport unique_ptr
from cython.operator cimport dereference as deref
from libc.stdlib cimport malloc, free

from rfbp cimport focusingBP, nonbayes_test
from pattern cimport Patterns
from fprotocol cimport FocusingProtocol
from mag cimport MagP64, MagT64

from ReplicatedFocusingBeliefPropagation.source.misc import _check_string
from enum import Enum
import numpy as np
cimport numpy as np


class Mag(Enum):
  magP = 0
  magT = 1




cdef class _FocusingProtocol:
  cdef unique_ptr[FocusingProtocol] thisptr

  cdef public:
    long int Nrep

  def __init__(self, other = None):
    try:
      protocol, size = other
      protocol = _check_string(protocol, exist=False)
      self.thisptr.reset(new FocusingProtocol(protocol, size))
    except:
      self.thisptr.reset(new FocusingProtocol())

    self.Nrep  = deref(self.thisptr).Nrep

  def __repr__(self):
    class_name = self.__class__.__name__
    return '<%s Class>'%(class_name)





cdef class _Pattern:
  cdef unique_ptr[Patterns] thisptr

  cdef public:
    long int Nrow, Ncol

  def __init__(self, other = None, binary=False, delimiter='\t'):

    try:
      N, M = other
      if not isinstance(N, int) or not isinstance(M, int):
        raise TypeError('N and M must be an integer')
      self.thisptr.reset(new Patterns(N, M))
    except ValueError:
      filename = other
      filename = _check_string(filename, exist=False)
      delimiter = _check_string(delimiter, exist=False)
      self.thisptr.reset(new Patterns(filename, binary, delimiter))
    except:
      self.thisptr.reset(new Patterns())

    self.Nrow = deref(self.thisptr).Nrow
    self.Ncol = deref(self.thisptr).Ncol

  @property
  def labels(self):
    labels = [deref(self.thisptr).output[i] for i in range(self.Nrow)]
    return labels

  @property
  def data(self):
    data = [[deref(self.thisptr).input[i][j] for j in range(self.Ncol)] for i in range(self.Nrow)]
    return data


  def __repr__(self):
    class_name = self.__class__.__name__
    return '<{} Class (Ninput: {} Nfeatures: {} >)'.format(class_name, self.Nrow, self.Ncol)






def _rfbp(mag, _Pattern pattern, _FocusingProtocol protocol, hidden = 3, max_iter = 1000, max_steps = 101, randfact = 1e-1, damping = 5e-1, epsil = 1e-1, accuracy = ['accurate', 'exact'], seed = 135, nth = 1) :
  acc1, acc2 = accuracy
  acc1 = _check_string(acc1, exist=False)
  acc2 = _check_string(acc2, exist=False)

  cdef long int **weights

  if mag == Mag.magP:
    weights = focusingBP[MagP64](hidden, deref(pattern.thisptr.get()), max_iter, max_steps, seed, damping, acc1, acc2, randfact, deref(protocol.thisptr.get()), epsil, nth)
  elif mag == Mag.magT:
    weights = focusingBP[MagT64](hidden, deref(pattern.thisptr.get()), max_iter, max_steps, seed, damping, acc1, acc2, randfact, deref(protocol.thisptr.get()), epsil, nth)
  else:
    raise TypeError('Invalid Magnetization given. Possible values are stored in Mag Enum {magP, magT}')

  return [[int(weights[i][j]) for j in range(pattern.Ncol)] for i in range(hidden)]

def _nonbayes_test(weights, _Pattern pattern, K):
  nlabel = pattern.ndata
  # convert variables
  row_size, column_size = np.shape(weights)

  cdef np.ndarray[long int, ndim=2, mode="c"] tmp_weights = np.ascontiguousarray(weights, dtype = np.int_)
  cdef long int ** weights_pointer = <long int **>malloc(row_size * sizeof(long int*))
  if not weights_pointer:
    raise MemoryError
  cdef int i
  cdef long int * predicted_labels
  try:
    for i in range(row_size):
      weights_pointer[i] = &tmp_weights[i, 0]

    # NOT WORKS BUT I DON'T KNOW WHY!!!
    predicted_labels = nonbayes_test(<long int **> &weights_pointer[0], deref(pattern.thisptr.get()), K)
    pred_labels = [int(predicted_labels[i]) for i in range(nlabel)]

  finally:
    free(weights_pointer)

  return pred_labels

