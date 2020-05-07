#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import numpy as np
from ReplicatedFocusingBeliefPropagation.rfbp.Mag import BaseMag
from ReplicatedFocusingBeliefPropagation.rfbp.atanherf import atanherf

__author__  = ['Nico Curti', "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class MagT64 (BaseMag):

  def __init__ (self, x, mInf=30.):
    '''
    MagP64 specialization

    Parameters
    ----------
      x : float
        Value to magnetify

      mInf : float (default = 30.)
        Clip value
    '''

    super(MagT64, self).__init__(x)
    self.mInf = mInf

  @property
  def value(self):
    return np.tanh(self.mag)

  @property
  def magformat (self):
    return 'tanh'

  @staticmethod
  def convert (x):
    return MagT64(np.clip(np.arctanh(x), -30., 30.))

  @staticmethod
  def couple (x1, x2):
    return MagT64.convert(np.log(x1 / x2) * .5)

  @staticmethod
  def mtanh (x):
    return MagT64(x)

  @staticmethod
  def merf (x):
    return MagT64( atanherf(x) )

  @BaseMag._require_mag
  def __mod__ (self, m):
    return self.__class__(self.mag + m.mag)

  @BaseMag._require_mag
  def __xor__ (self, m):

    ax, ay = (self.mag, m.mag)

    if ax >= ay and ax >= -ay:
      t1 = 2. * ay
    elif ax >= ay and ax < -ay:
      t1 = -2. * ay
    elif ax < ay and ax >= -ay:
      t1 = 2. * ax
    else:
      t1 = -2. * ay

    if np.isinf(ax) or np.isinf(ay):
      t2 = 0.
    else:
      t2 = np.log((np.exp(2 * np.abs(ax + ay)) + 1) / (np.exp(2 * np.abs(ax - ay)) + 1)) + 2 * np.abs(ax - ay) - 2 * np.abs(ax + ay)

    return self.__class__(np.mean((t1, t2)))
