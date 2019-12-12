#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import numpy as np
from sklearn.utils import check_X_y
from sklearn.utils import check_array
from lib.ReplicatedFocusingBeliefPropagation.rFBP import _Pattern
from ReplicatedFocusingBeliefPropagation.rfbp.misc import _check_string

__all__ = ['Pattern']

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

class Pattern (_Pattern):

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
      y = y.astype('int64')


      self._pattern = _Pattern(X=X, y=y, M=M, N=N)

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
      raise ValueError('Incorrect dimensions. M and N must be greater than 0. Given ({}, {})'.format(M, N))

    self._pattern = _Pattern(M=M, N=N)

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
      raise InputError('Invalid filename found. Filename must be a string. Given : {}'.format(filename))

    filename = _check_string(filename, exist=True)
    delimiter = _check_string(delimiter, exist=False)

    self._pattern = _Pattern(filename=filename, binary=binary, delimiter=delimiter)

    return self

  @property
  def shape (self):
    try:
      return (self._pattern.Nrow, self._pattern.Ncol)

    except AttributeError:
      return (0, 0)

  @property
  def labels (self):
    try:
      return self._pattern.labels

    except AttributeError:
      return None

  @property
  def data (self):
    try:
      return self._pattern.data

    except AttributeError:
      return None

  @property
  def pattern (self):
    return self._pattern

  def __repr__ (self):
    class_name = self.__class__.__qualname__
    if self._pattern is not None:
      return '{0}().random(shapes=({1:d}, {2:d}))'.format(class_name, self._pattern.Nrow, self._pattern.Ncol)

    else:
      return '{0}().random(shapes=(0, 0))'.format(class_name)


if __name__ == '__main__':

  n_sample = 10
  n_feature = 20

  data = np.random.choice(a=(-1, 1), p=(.5, .5), size=(n_sample, n_feature))
  labels = np.random.choice(a=(-1, 1), p=(.5, .5), size=(n_sample,))

  pt = Pattern(X=data, y=labels)
  print(pt)

  # constructor
  assert pt.pattern is not None

  # dimensions
  assert pt.shape == (n_sample, n_feature)

  # data
  assert np.allclose(pt.data, data)

  # labels
  assert np.allclose(pt.labels, labels)
