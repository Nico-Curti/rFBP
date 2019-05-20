#!/usr/bin/env python

from lib.ReplicatedFocusingBeliefPropagation.rFBP import _Pattern

__package__ = "Pattern_wrap"
__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

class Pattern(_Pattern):

  def __init__(self, X=None, y=None, binary=False, delimiter='\t'):
    if not X is None:
      if not y is None:
        self._pattern = _Pattern(other=[X, y])
      elif isinstance(X, str):
        self._pattern = _Pattern(other=X, binary=binary, delimiter=delimiter)
      else:
        raise ValueError('Wrong input variable supplied. Either X must be a string or (X, y) must be (number, number) or (array-like, array-like).')
      self.nfeature = self._pattern.Ncol
      self.ndata    = self._pattern.Nrow
      self._labels  = self._pattern.labels
      self._data    = self._pattern.data
    else:
      self._pattern = _Pattern()
      self.nfeature = 0
      self.ndata = 0

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
    return '<{0} Class (Ndata: {1} Nfeatures: {2}) >'.format(class_name, self.ndata, self.nfeature)
