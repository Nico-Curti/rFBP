#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import pytest
import numpy as np
from ReplicatedFocusingBeliefPropagation.rfbp.Mag import BaseMag

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class TestMag:
  '''
  Test MagT64
  '''
  np.random.seed(42)

  x = np.random.uniform(low=0., high=1)
  y = np.random.uniform(low=0., high=1)
  m1 = BaseMag(x)
  m2 = BaseMag(y)

  def test_value (self):
    with pytest.raises(NotImplementedError):
      self.m1.value

  def test_magformat (self):
    with pytest.raises(NotImplementedError):
      self.m1.magformat

  def test_mod_operator (self):
    with pytest.raises(NotImplementedError):
      x = self.m1 % self.m2
      assert x

  def test_xor_operator (self):
    with pytest.raises(NotImplementedError):
      x = self.m1 ^ self.m2
      assert x
