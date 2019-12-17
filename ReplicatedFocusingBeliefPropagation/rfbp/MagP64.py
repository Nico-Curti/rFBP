#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import numpy as np
from scipy.special import erf
from ReplicatedFocusingBeliefPropagation.rfbp.Mag import BaseMag

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class MagP64 (BaseMag):

  def __init__ (self, x):

    super(MagP64, self).__init__(x)

  @property
  def value (self):
    return self.mag

  @property
  def magformat (self):
    return 'plain'

  @staticmethod
  def convert (x):
    return MagP64(x)

  @staticmethod
  def couple (x1, x2):
    return MagP64( (x1 - x2)/(x1 + x2) )

  @staticmethod
  def mtanh (x):
    return MagP64(np.tanh(x))

  @staticmethod
  def merf (x):
    return MagP64(erf(x))

  @BaseMag._require_mag
  def __mod__ (self, m):
    return self.__class__(np.clip( (self.mag + m.mag) / (1. + self.mag * m.mag), -1., 1.))

  @BaseMag._require_mag
  def __xor__ (self, m):
    return self.__class__(self.mag * m.mag)

