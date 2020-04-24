#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

from ReplicatedFocusingBeliefPropagation.lib.FocusingProtocol import _FocusingProtocol
from ReplicatedFocusingBeliefPropagation.rfbp.misc import _check_string

__all__ = ['Focusing_Protocol']

__author__  = ['Nico Curti', "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class Focusing_Protocol (object):

  '''
  Focusing Protocol object

  Parameters
  ----------
    protocol : string
      The value of string can be only one of ['scoping', 'pseudo_reinforcement', 'free_scoping', 'standard_reinforcement']

    size : int (default = 101)
      Dimension of update protocol

  Members
  -------
    num_of_replicas : Return the number of replicas

    fprotocol : Return the Cython FocusingProtocol object

    gamma : Return the array of gamma values

    n_rep : Return the array of n_rep values

    beta : Return the array of beta values
  '''

  ALLOWED_PROTOCOL = ('scoping', 'pseudo_reinforcement', 'free_scoping', 'standard_reinforcement')

  def __init__ (self, protocol='standard_reinforcement', size=101):

    if protocol not in self.ALLOWED_PROTOCOL:
      raise ValueError('Incorrect Protocol found. Possible values are only {}'.format(','.join(self.ALLOWED_PROTOCOL)))

    if size <= 1:
      raise ValueError('Incorrect size. Size must be > than 1. Given {}'.format(size))

    self._protocol = protocol
    byte_protocol = _check_string(protocol, exist=False)
    self._fprotocol = _FocusingProtocol(protocol=byte_protocol, size=size)

    self._nrep = self._fprotocol.Nrep

  @property
  def num_of_replicas (self):
    '''
    Return the number of replicas
    '''
    return self._nrep

  @property
  def fprotocol (self):
    '''
    Return the Cython object
    '''
    return self._fprotocol

  @property
  def gamma (self):
    '''
    Return the 'gamma' array
    '''
    return self._fprotocol.gamma

  @property
  def n_rep (self):
    '''
    Return the 'n_rep' array
    '''
    return self._fprotocol.n_rep

  @property
  def beta (self):
    '''
    Return the 'beta' array
    '''
    return self._fprotocol.beta

  def __repr__ (self):
    '''
    Object representation
    '''
    class_name = self.__class__.__qualname__
    return '{0}(protocol={1}, size={2:d})'.format(class_name, self._protocol, self._nrep)
