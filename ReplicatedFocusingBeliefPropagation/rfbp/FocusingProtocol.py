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
  Focusing Protocol object.
  Abstract type representing a protocol for the focusing procedure, i.e. a way to produce successive values for the quantities γ, y and β.
  Currently, however, only β=Inf is supported.
  To be provided as an argument to focusingBP.

  Available protocols are: StandardReinforcement, Scoping, PseudoReinforcement and FreeScoping.

  - StandardReinforcement: returns γ=Inf and y=1/(1-x), where x is taken from the given range r.
  - Scoping: fixed y and a varying γ taken from the given γr range.
  - PseudoReinforcement: both γ and y are progressively increased, according to the formulas

  >>> γ = atanh(ρ**x)
  >>> y = 1+ρ**(1-2*x)/(1-ρ)

  where ρ is taken from the given range(s) r.
  With x=0, this is basically the same as StandardReinforcement.

  - FreeScoping: just returns the values of (γ,y) from the given list


  Parameters
  ----------
    protocol : string
      The value of string can be only one of ['scoping', 'pseudo_reinforcement', 'free_scoping', 'standard_reinforcement']

    size : int (default = 101)
      Dimension of update protocol

  Example
  -------
  >>> from ReplicatedFocusingBeliefPropagation import Focusing_Protocol
  >>>
  >>> fprotocol = Focusing_Protocol('scoping', 101)
  >>> fprotocol
    Focusing_Protocol(protocol=scoping, size=101)
  '''

  _ALLOWED_PROTOCOL = ('scoping', 'pseudo_reinforcement', 'free_scoping', 'standard_reinforcement')

  def __init__ (self, protocol='standard_reinforcement', size=101):

    if protocol not in self._ALLOWED_PROTOCOL:
      raise ValueError('Incorrect Protocol found. Possible values are only {}'.format(','.join(self._ALLOWED_PROTOCOL)))

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

    Returns
    -------
      nrep: int
        The number of replicas
    '''
    return self._nrep

  @property
  def fprotocol (self):
    '''
    Return the Cython object

    Returns
    -------
      fprotocol: Cython object
        The cython object wrapped by the Pattern class

    Notes
    -----
    .. warning::
      We discourage the use of this property if you do not know exactly what you are doing!
    '''
    return self._fprotocol

  @property
  def gamma (self):
    '''
    Return the 'gamma' array

    Returns
    -------
      gamma: array-like
        The vector of the gamma values
    '''
    return self._fprotocol.gamma

  @property
  def n_rep (self):
    '''
    Return the 'n_rep' array

    Returns
    -------
      n_rep: array-like
        The vector of the n_rep values
    '''
    return self._fprotocol.n_rep

  @property
  def beta (self):
    '''
    Return the 'beta' array

    Returns
    -------
      beta: array-like
        The vector of the beta values
    '''
    return self._fprotocol.beta

  def __repr__ (self):
    '''
    Object representation
    '''
    class_name = self.__class__.__qualname__
    return '{0}(protocol={1}, size={2:d})'.format(class_name, self._protocol, self._nrep)
