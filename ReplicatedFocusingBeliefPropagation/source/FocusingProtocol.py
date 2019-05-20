#!/usr/bin/env python

from lib.ReplicatedFocusingBeliefPropagation.rFBP import _FocusingProtocol

__package__ = "FocusingProtocol_wrap"
__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class Focusing_Protocol(_FocusingProtocol):

  def __init__(self, protocol = None, size = 101):
    if protocol:
      self.protocol = protocol
      self._fprotocol = _FocusingProtocol(other=[protocol, size])
    else:
      self.protocol = 'unknown'
      self._fprotocol = _FocusingProtocol()

    self.nrep = self._fprotocol.Nrep

  @property
  def fprotocol(self):
    return self._fprotocol

  def __repr__(self):
    class_name = self.__class__.__name__
    return '<{0} Class (protocol: {1}, number of replicas: {2}) >'.format(class_name, self.protocol, self.nrep)
