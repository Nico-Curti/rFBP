#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

from lib.ReplicatedFocusingBeliefPropagation.rFBP import _Pattern

__package__ = "Pattern_wrap"
__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

class Pattern(_Pattern):

  def __init__(self, X=None, y=None, binary=False, delimiter='\t'):
    '''
    Pattern object for C++ compatibility

    Parameters
    ----------
      X : None or 2D array-like or string
        Input matrix of variabels as (Nsample, Nfeatures) or filename with the input stored in the same way

      y : None or 1D array-like
        Input labels. The label can be given or read from the input filename as first row in the file.

      binary : bool
        Switch between binary or ascii format in the input file

      delimiter : char
        Delimiter char of the ascii file

    Members
    -------
      shape : Return the shape of input data as (nsample, nfeatures)

      labels : Get the array of labels stored inside the object

      data : Return the full matrix of data

      pattern : Return the Cython Pattern object
    '''
    if not X is None:

      if not y is None:
        self._pattern = _Pattern(other=[X, y])

      elif isinstance(X, str):
        self._pattern = _Pattern(other=X, binary=binary, delimiter=delimiter)

      else:
        raise ValueError('Wrong input variable supplied. Either X must be a string or (X, y) must be (number, number) or (array-like, array-like).')

      self._nfeature = self._pattern.Ncol
      self._ndata    = self._pattern.Nrow
      self._labels  = self._pattern.labels
      self._data    = self._pattern.data

    else:

      self._pattern = _Pattern()
      self._nfeature = 0
      self._ndata = 0

  @property
  def shape(self):
    return (self._ndata, self._nfeature)

  @property
  def labels(self):
    return self._labels

  @property
  def data(self):
    return self._data

  @property
  def pattern(self):
    return self._pattern

  def __repr__(self):
    class_name = self.__class__.__name__
    return '<{0} Class (Ndata: {1}, Nfeatures: {2}) >'.format(class_name, self._ndata, self._nfeature)
