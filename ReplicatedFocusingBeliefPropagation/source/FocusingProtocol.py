#!/usr/bin/env python

from lib.ReplicatedFocusingBeliefPropagation.rFBP import _FocusingProtocol

__package__ = "FocusingProtocol_wrap"
__author__  = ["Nico Curti (nico.curit2@unibo.it)", "Daniele Dall'Olio (daniele.dallolio@studio.unibo.it)"]


class Focusing_Protocol(_FocusingProtocol):

  def __init__(self, protocol = None, size = 101):
    if protocol:
      self.protocol = protocol
      self.__fprotocol = _FocusingProtocol(other=[protocol, size])
    else:
      self.protocol = 'unknown'
      self.__fprotocol = _FocusingProtocol()

    self.nrep = self.__fprotocol.Nrep

  @property
  def fprotocol(self):
    return self.__fprotocol

  def __repr__(self):
    class_name = self.__class__.__name__
    return '<{0} Class (protocol: {1}, number of replicas: {2}) >'.format(class_name, self.protocol, self.nrep)


