#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import numpy as np
from .Mag import BaseMag
# miss import magnetization

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class MagT64 (BaseMag):

  def __init__ (self, x, m=30):

    super(MagT64, self).__init__(x)
    self.mInf = m

  @property
  def value(self):
    return np.tanh(self.mag)

  @property
  def magformat (self):
    return 'tanh'

  @BaseMag._require_mag
  def __mod__ (self, m):
    return self.__class__(self.mag + m.mag)

  @BaseMag._require_mag
  def __xor__ (self, m):

    ax, ay = map(abs, (self.mag, m.mag))
    t1 = None

    t2 = 0. if np.isinf(ax) or np.isinf(ay) else lr(ax + ay) - lr(ax - ay)

    return self.__class__(np.mean((t1, t2)))



if __name__ == '__main__':

  pass
