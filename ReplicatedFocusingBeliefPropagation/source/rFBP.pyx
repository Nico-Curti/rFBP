# distutils: language = c++
# cython: language_level=2

from libcpp.memory cimport unique_ptr
from cython.operator cimport dereference as deref
from libc.stdlib cimport malloc, free
cimport numpy as np

from rfbp cimport focusingBP, nonbayes_test
from pattern cimport Patterns
from fprotocol cimport FocusingProtocol
from mag cimport MagP64, MagT64
from misc cimport double_pointers_for_cython

from ReplicatedFocusingBeliefPropagation.rfbp.misc import _check_string
import numpy as np
import numbers
from sklearn.utils import check_X_y, check_array
import warnings

from enum import Enum

class Mag(Enum):
  magP = 0
  magT = 1



cdef class _FocusingProtocol:
  cdef unique_ptr[FocusingProtocol] thisptr

  cdef public:
    long int Nrep

  def __init__(self, other=None):
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

  def __init__(self, other=None, binary=False, delimiter='\t'):
    cdef int m, n
    cdef np.ndarray[double, ndim=2, mode="c"] X
    cdef np.ndarray[long int, ndim=1, mode="c"] y

    if other is None:

      self.thisptr.reset(new Patterns())

    else:
      *_X, _y = other

      if isinstance(*_X, numbers.Number) and isinstance(_y, numbers.Number):
        _X = _X[0]

        if not isinstance(_X, int) or not isinstance(_y, int):
          warnings.warn('N and M are supposed to be integers.')
          _X, _y = int(_X), int(_y)
        self.thisptr.reset(new Patterns(_X, _y))

      elif not _X and isinstance(_y, str):
        filename = _y
        filename = _check_string(filename, exist=False)
        delimiter = _check_string(delimiter, exist=False)
        self.thisptr.reset(new Patterns(filename, binary, delimiter))

      elif len(_X) and len(_y):
        _X, _y = check_X_y(*_X, _y)
        n, m = _X.shape
        X = _X.astype('float64')
        y = _y
        self.thisptr.reset(new Patterns(double_pointers_for_cython[double,double](&X[0,0], n, m), &y[0], n, m))

      else:
        raise ValueError('Wrong input variable supplied. Either X must be a string or (X, y) must be (number, number) or (array-like, array-like).')

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






def _rfbp(mag, _Pattern pattern, _FocusingProtocol protocol, hidden=3, max_iter=1000, max_steps=101, randfact=1e-1, damping=5e-1, epsil=1e-1, accuracy=['accurate', 'exact'], seed=135, nth=1) :
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
  nlabel = pattern.Nrow
  weights = check_array(weights)

  cdef int row_size, column_size
  cdef np.ndarray["long int", ndim=2, mode="c"] weights_pointer = weights
  row_size, column_size = weights.shape

  cdef long int ** c_weights  = double_pointers_for_cython["long int","long int"](&weights_pointer[0,0], row_size, column_size)
  cdef long int * predicted_labels = nonbayes_test(c_weights, pattern.thisptr.get()[0], K)

  pred_labels = np.asarray([int(predicted_labels[i]) for i in range(nlabel)])

  return pred_labels
