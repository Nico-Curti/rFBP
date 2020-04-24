# distutils: language = c++
# cython: language_level=2

from libcpp.memory cimport unique_ptr
from cython.operator cimport dereference as deref
cimport numpy as np

from rfbp cimport focusingBP
from rfbp cimport nonbayes_test
from Patterns cimport _Patterns
from FocusingProtocol cimport _FocusingProtocol
from MagP64 cimport MagP64
from MagT64 cimport MagT64
from misc cimport double_pointers_for_cython

from enum import Enum


class Mag (int, Enum):
  MagP64 = 0
  MagT64 = 1


def _rfbp (int mag, _Patterns pattern, _FocusingProtocol protocol,
           long int hidden=3, long int max_iter=1000, long int max_steps=101, double randfact=1e-1,
           double damping=5e-1, double epsil=1e-1, accuracy=(b'accurate', b'exact'),
           long int seed=135, long int nth=1):

  acc1, acc2 = accuracy

  cdef long int ** weights

  if mag == Mag.MagP64:
    weights = focusingBP[MagP64](hidden, deref(pattern.thisptr.get()), max_iter, max_steps, seed, damping, acc1, acc2, randfact, deref(protocol.thisptr.get()), epsil, nth)

  elif mag == Mag.MagT64:
    weights = focusingBP[MagT64](hidden, deref(pattern.thisptr.get()), max_iter, max_steps, seed, damping, acc1, acc2, randfact, deref(protocol.thisptr.get()), epsil, nth)

  else:
    raise TypeError('Invalid Magnetization given. Possible values are stored in Mag Enum {magP, magT}')

  return [[int(weights[i][j]) for j in range(pattern.Ncol)] for i in range(hidden)]


def _nonbayes_test (long int[::1] weights, long int row_size, long int column_size, _Patterns pattern, long int K):

  nlabel = pattern.Nrow

  cdef long int ** c_weights = double_pointers_for_cython['long int', 'long int'](&weights[0], row_size, column_size)
  cdef long int * predicted_labels = nonbayes_test(c_weights, pattern.thisptr.get()[0], K)

  return [int(predicted_labels[i]) for i in range(nlabel)]
