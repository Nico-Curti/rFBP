#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import numpy as np
from scipy.special import erf
from ReplicatedFocusingBeliefPropagation import atanherf
from ReplicatedFocusingBeliefPropagation import MagT64

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class TestMagT64:
  '''
  Test MagT64
  '''
  np.random.seed(42)

  x = np.random.uniform(low=0., high=1)
  y = np.random.uniform(low=0., high=1)
  m1 = MagT64(x)
  m2 = MagT64(y)
  null = MagT64(0.)
  one = MagT64(1.)

  def test_value (self):
    assert np.isclose(self.m1.mag, self.x)
    assert np.isclose(self.m2.mag, self.y)
    assert np.isclose(self.m1.value, np.tanh(self.x))
    assert np.isclose(self.m2.value, np.tanh(self.y))

  def test_copy (self):
    m = MagT64(self.x)
    assert np.isclose(m.mag, self.x)
    assert np.isclose(m.value, np.tanh(self.x))
    assert m == self.m1

  def test_fmt (self):
    assert self.m1.magformat == 'tanh'
    assert self.m2.magformat == 'tanh'

  def test_convert (self):
    x = MagT64.convert(self.x)
    assert -30. <= x.mag <= 30.
    assert np.isclose(x.mag, np.arctanh(self.x))
    assert np.isclose(x.value, self.x)

  def test_couple (self):
    x = MagT64.couple(self.x, self.y)
    y = MagT64.couple(self.y, self.x)
    assert np.isclose(abs(x.mag), abs(y.mag))
    assert np.isclose(abs(x.value), abs(y.value))

  def test_mtanh (self):
    x = MagT64.mtanh(self.x)
    assert np.isclose(x.mag, self.x)
    assert np.isclose(x.value, np.tanh(self.x))

  def test_merf (self):
    x = MagT64.merf(self.x)
    assert np.isclose(x.mag, atanherf(self.x))
    assert np.isclose(x.value, erf(self.x))

  def test_mod_operator (self):
    x = self.m1 % self.m2
    y = self.m2 % self.m1
    assert np.isclose(x.mag, y.mag)
    assert np.isclose(x.value, y.value)

    x = self.m1 % self.null
    assert np.isclose(x.mag, self.m1.mag)
    assert np.isclose(x.value, self.m1.value)

  def test_add_operator (self):
    x = self.m1 + self.m2
    y = self.m2 + self.m1

    assert np.isclose(y.mag, x.mag)
    assert np.isclose(y.value, x.value)

    x = self.m1 + self.null
    assert np.isclose(x.mag, self.m1.mag)
    assert np.isclose(x.value, self.m1.value)

  def test_div_operator (self):
    x = self.m1 / self.y
    assert np.isclose(x.mag, self.m1.mag / self.y)
    assert np.isclose(x.value, np.tanh(self.m1.mag / self.y))

    x = self.m1 / self.x
    assert np.isclose(x.mag, 1.)
    assert np.isclose(x.value, np.tanh(1.))

    # x = self.m1 / 0.
    # assert np.isclose(x.mag, float('inf'))
    # assert np.isclose(x.value, float('inf'))

  def test_mul_operator (self):
    x = self.m1 * self.y
    assert np.isclose(x, self.m1.mag * self.y)

    x = self.m1 * 1.
    assert np.isclose(x, self.m1.mag)

    x = self.m1 * 0.
    assert np.isclose(x, 0.)

    # x = self.y * self.m1
    # assert np.isclose(x, self.m1.mag * self.y)

    # x = 1. * self.m1
    # assert np.isclose(x, self.m1.mag)

    # x = 0. * self.m1
    # assert np.isclose(x, 0.)

  def test_xor_operator (self):
    x = self.m1 ^ self.m2
    y = self.m2 ^ self.m1
    assert np.isclose(x.mag, y.mag)
    assert np.isclose(x.value, y.value)

    x = (-self.m1) ^ (-self.m2)
    y = (-self.m2) ^ (-self.m1)
    assert not np.isclose(x.mag, y.mag)
    assert not np.isclose(x.value, y.value)

    x = self.m1 ^ self.null
    assert np.isclose(x.mag, 0.)
    assert np.isclose(x.value, 0.)

    x = self.m1 ^ MagT64(float('inf'))
    assert np.isclose(x.mag, self.m1.mag)
    assert np.isclose(x.value, self.m1.value)

  def test_sub_operator (self):
    x = self.m1 - self.m2

    assert np.isclose(self.m1.value, x + self.m2.value)

    x = self.m2 - self.m1
    assert np.isclose(self.m2.value, x + self.m1.value)

    x = self.m1 - self.null
    assert np.isclose(x, self.m1.value)

  def test_eq_operator (self):
    assert (self.m1 == self.m2) == (self.m2 == self.m1)
    assert (self.m1 != self.m2) == (self.m2 != self.m1)
