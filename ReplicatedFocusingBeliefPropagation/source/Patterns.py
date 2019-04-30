#!/usr/bin/env python

from lib.ReplicatedFocusingBeliefPropagation.rFBP import _Pattern

__package__ = "Pattern_wrap"
__author__  = ["Nico Curti (nico.curit2@unibo.it)", "Daniele Dall'Olio (daniele.dallolio@studio.unibo.it)"]

class Pattern(_Pattern):

  def __init__(self, X = None, y = None):
    if X and y:
      self.__pattern = _Pattern(other=[X, y]) # TODO
    else:
      self.nfeature = 0
      self.ndata = 0

  def load(self, filename, binary=False, delimiter='\t'):
    self.__pattern = _Pattern(other=filename, binary=binary, delimiter=delimiter)
    self.nfeature  = self.__pattern.Ncol
    self.ndata     = self.__pattern.Nrow
    self.__labels  = self.__pattern.labels
    self.__data    = self.__pattern.data

  def random(self, N, M):
    self.__pattern = _Pattern(other=[N, M])
    self.nfeature  = self.__pattern.Ncol
    self.ndata     = self.__pattern.Nrow
    self.__labels  = self.__pattern.labels
    self.__data    = self.__pattern.data

  @property
  def labels(self):
    return self.__labels

  @property
  def data(self):
    return self.__data

  @property
  def pattern(self):
    return self.__pattern

  def __repr__(self):
    class_name = self.__class__.__name__
    return '<{0} Class (Ndata: {1} Nfeatures: {2}) >'.format(class_name, self.ndata, self.nfeature)


