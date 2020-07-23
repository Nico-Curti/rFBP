#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import numpy as np
from sklearn.utils import check_X_y
from sklearn.utils import check_array
from ReplicatedFocusingBeliefPropagation.lib.Patterns import _Patterns
from ReplicatedFocusingBeliefPropagation.rfbp.misc import _check_string
from ReplicatedFocusingBeliefPropagation.rfbp.misc import get_int_size

__all__ = ['Pattern']

__author__  = ['Nico Curti', "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

class Pattern (object):

  '''
  Pattern object for C++ compatibility

  Parameters
  ----------
    X : None or 2D array-like or string
      Input matrix of variabels as (Nsample, Nfeatures) or filename with the input stored in the same way

    y : None or 1D array-like
      Input labels. The label can be given or read from the input filename as first row in the file.

  Members
  -------
    shape : Return the shape of input data as (nsample, nfeatures)

    labels : Get the array of labels stored inside the object

    data : Return the full matrix of data

    pattern : Return the Cython Pattern object

    load : Load pattern from file

  '''

  def __init__ (self, X=None, y=None):

    if X is not None and y is not None:

      # check array
      X, y = check_X_y(X, y)
      N, M = X.shape

      X = check_array(X)
      X = X.ravel()

      X = np.ascontiguousarray(X)
      y = np.ascontiguousarray(y)

      X = X.astype('float64')
      y = y.astype(get_int_size())

      self._pattern = _Patterns(X=X, y=y, M=M, N=N)
      self._check_binary()

    else:

      self._pattern = None

  def random (self, shape):
    '''
    Generate Random pattern

    Parameters
    ----------
      shapes : tuple
        a 2-D tuple with (M, N) where M is the number of samples and N the number of probes
    '''

    try:
      M, N = map(int, shape)

    except ValueError:
      raise ValueError('Incorrect dimensions. Shapes must be a 2-D tuple with (M, N)')

    if M <= 0 or N <= 0:
      raise ValueError('Incorrect dimensions. M and N must be greater than 0. Given ({0:d}, {1:d})'.format(M, N))

    self._pattern = _Patterns(M=M, N=N)

    # We do not need to check the variables since they are correctly generated into the C++ code!

    return self


  def load (self, filename, binary=False, delimiter='\t'):
    '''
    Load pattern from file

    Parameters
    ----------
      filename : str
        Filename/Path to the Pattern file

      binary : bool
        True if the filename is in binary fmt; False for ASCII fmt

      delimiter : str
        Separator of input file (valid if binary is False)
    '''

    if not isinstance(filename, str):
      raise ValueError('Invalid filename found. Filename must be a string. Given : {0}'.format(filename))

    filename = _check_string(filename, exist=True)
    delimiter = _check_string(delimiter, exist=False)

    self._pattern = _Patterns(filename=filename, binary=binary, delimiter=delimiter)
    self._check_binary()

    return self

  @property
  def shape (self):
    '''
    Return the shape of the data matrix
    '''
    try:
      return (self._pattern.Nrow, self._pattern.Ncol)

    except AttributeError:
      return (0, 0)

  @property
  def labels (self):
    '''
    Return the label array
    '''
    try:
      return np.asarray(self._pattern.labels, dtype=int)

    except AttributeError:
      return None

  @property
  def data (self):
    '''
    Return the data matrix
    '''
    try:
      return np.asarray(self._pattern.data, dtype=int)

    except AttributeError:
      return None

  @property
  def pattern (self):
    '''
    Return the pattern Cython object
    '''
    return self._pattern

  def _check_binary (self):
    '''
    Check if the input data and labels satisfy the binary
    requirements
    '''

    if not (((-1 == self.data) | (1 == self.data)).all() or ((-1 == self.labels) | (1 == self.labels)).all()):
      self._pattern = None # remove the loaded object
      raise ValueError('Invalid input parameters! Input variables must be +1 or -1')

  def __repr__ (self):
    '''
    Object representation
    '''
    class_name = self.__class__.__qualname__
    if self._pattern is not None:
      return '{0}[shapes=({1:d}, {2:d})]'.format(class_name, self._pattern.Nrow, self._pattern.Ncol)

    else:
      return '{0}[shapes=(0, 0)]'.format(class_name)
