#!/usr/bin/env python
# distutils: language = c++
# distutils: sources = fbp.cpp
# cython: language_level=3

cimport cython
from libcpp.string cimport string
from libcpp.memory cimport unique_ptr
from cython.operator cimport dereference as deref
import numpy as np
from ._mag import *
from .Fprotocol import FocusingProtocol, FProtocol
from .Pattern import Patterns, Pattern

cdef extern from "rfbp.h":
  cdef void focusingBP(long int K,
                       object patterns,
                       long int max_iters,
                       long int max_steps,
                       long int seed,
                       double damping,
                       string accuracy1,
                       string accuracy2,
                       double randfact,
                       object fprotocol,
                       double epsil,
                       string outfile,
                       string outmessfiletmpl,
                       string initmess)





class ReplicatedFocusingBeliefPropagation(object):

  def __init__(self):
    pass


  def _predict(self, X):
    return

  def predict(self, X):
    #check_is_fitted(self, "coefs_")
    y_pred = self._predict(X)

    if self.n_outputs_ == 1:
      y_pred = y_pred.ravel()

    return y_pred

  def _fit(self, X, y):
    return

  def fit(self, X, y):
    return self._fit(X, y)

  def predict_log_proba(self, X):
    y_prob = self.predict_proba(X)
    return np.log(y_prob, out=y_prob)

  def predict_proba(self, X):
    #check_is_fitted(self, "coefs_")
    y_pred = self._predict(X)

    if self.n_outputs_ == 1:
      y_pred = y_pred.ravel()

    if y_pred.ndim == 1:
      return np.vstack([1 - y_pred, y_pred]).T
    else:
      return y_pred




  def __repr__(self):
    class_name = self.__class__.__name__
    return '<%s Class>'%(class_name)


  @cython.cdivision(True)
  @cython.boundscheck(False)
  @cython.wraparound(False)
  def fbp(self,
          string trainfile,
          int hidden,
          int max_iter,
          double randfact,
          double damping,
          string accuracy1,
          string accuracy2,
          string protocol,
          int seed) :

    trainfile = str.encode(trainfile)
    accuracy1 = str.encode(accuracy1)
    accuracy2 = str.encode(accuracy2)
    protocol  = str.encode(protocol)