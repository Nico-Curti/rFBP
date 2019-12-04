#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

from lib.ReplicatedFocusingBeliefPropagation.rFBP import _FocusingProtocol

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class Focusing_Protocol(_FocusingProtocol):

  def __init__(self, protocol=None, size=101):
    '''
    Focusing Protocol object

    Parameters
    ----------
      protocol : None (default) or string
        The value of string can be only one of ['scoping', 'pseudo_reinforcement', 'free_scoping', 'standard_reinforcement']

      size : int (default = 101)
        Dimension of update protocol

    Members
    -------
      num_of_replicas : Return the number of replicas

      fprotocol : Return the Cython FocusingProtocol object
    '''

    if protocol:

      if protocol not in ['scoping', 'pseudo_reinforcement', 'free_scoping', 'standard_reinforcement']:
        raise ValueError('Incorrect Protocol found. Possible values are only ["scoping", "pseudo_reinforcement", "free_scoping", "standard_reinforcement"]')

      self._protocol = protocol
      self._fprotocol = _FocusingProtocol(other=[protocol, size])

    else:

      self._protocol = 'unknown'
      self._fprotocol = _FocusingProtocol()

    self._nrep = self._fprotocol.Nrep

  @property
  def num_of_replicas(self):
    return self._nrep

  @property
  def fprotocol(self):
    return self._fprotocol

  def __repr__(self):
    class_name = self.__class__.__name__
    return '<{0} Class (protocol: {1}, number of replicas: {2}) >'.format(class_name, self._protocol, self._nrep)
