#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import pytest
import numpy as np
from ReplicatedFocusingBeliefPropagation import magnetization as mag
from ReplicatedFocusingBeliefPropagation import MagP64
from ReplicatedFocusingBeliefPropagation import MagT64

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class TestMagnetization:
  '''
  Test Magnetization functions
  '''

  x = np.random.uniform(low=0., high=1.)
  y = np.random.uniform(low=0., high=1.)
  m = MagP64(np.random.uniform(low=0., high=1.))
  n = MagP64(np.random.uniform(low=0., high=1.))
  mt = MagT64(np.random.uniform(low=0., high=1.))
  nt = MagT64(np.random.uniform(low=0., high=1.))

  def test_check_mag (self):

    with pytest.raises(ValueError):
      mag._check_mag(3.14)

  def test_lr (self):
    # TODO: increase test
    assert mag.lr(self.x) >= 0.

  def test_sign0 (self):
    assert mag.sign0(self.x) in (-1, 1)
    assert mag.sign0(-self.x) == 1
    assert mag.sign0(self.m) in (-1, 1)

  def test_zeros (self):
    mags = [MagP64(_) for _ in range(10)]
    mag.zeros(mags)

    assert all((i.mag == 0 for i in mags))

    with pytest.raises(ValueError):
      l = 3.14
      mag.zeros(3.14)

    with pytest.raises(ValueError):
      l = [MagP64(3.14), self.x]
      mag.zeros(l)

  def test_zero (self):
    m = MagP64(3.14)
    mag.zero(m)

    assert m.mag == 0.
    assert m.value == 0.

    with pytest.raises(ValueError):
      mag.zero(self.x)

  def test_mabs (self):
    assert mag.mabs(MagP64(self.x)) >= 0.
    assert mag.mabs(MagP64(-self.x)) >= 0.
    assert mag.mabs(MagP64(-self.x)) == mag.mabs(MagP64(self.x))

    with pytest.raises(ValueError):
      mag.mabs(self.x)

  def test_copysign (self):
    x = mag.copysign(self.m, self.x)
    assert x.mag == self.m.mag

    x = mag.copysign(-self.m, self.x)
    assert x.mag == self.m.mag

    x = mag.copysign(self.m, -self.x)
    assert x.mag == -self.m.mag

    x = mag.copysign(-self.m, -self.x)
    assert x.mag == -self.m.mag

  def test_arrow (self):

    x = mag.arrow(self.m, self.x)
    assert isinstance(x, MagP64)

    x = mag.arrow(self.mt, self.x)
    assert isinstance(x, MagT64)

    with pytest.raises(ValueError):
      mag.arrow(self.x, self.y)

  def test_logmag2p (self):

    x = mag.logmag2p(MagP64(0.))
    assert np.isclose(x, np.log(.5))

    with pytest.warns(RuntimeWarning):
      x = mag.logmag2p(MagT64(-1.))
      assert np.isinf(x)

  def test_damp (self):

    x = mag.damp(self.m, self.n, self.x)
    y = mag.damp(self.n, self.m, 1. - self.x)
    assert np.isclose(x.mag, y.mag)
    assert np.isclose(x.value, y.value)

    x = mag.damp(self.m, self.n, 0.)
    assert np.isclose(x.mag, self.m.mag)

    x = mag.damp(self.m, self.n, 1.)
    assert np.isclose(x.mag, self.n.mag)

    x = mag.damp(self.mt, self.nt, 0.)
    assert np.isclose(x.mag, self.mt.mag)

    x = mag.damp(self.mt, self.nt, 1.)
    assert np.isclose(x.mag, self.nt.mag)

    with pytest.raises(ValueError):
      mag.damp(self.x, self.y, self.m)


  def test_bar (self):

    x = mag.bar(self.mt, self.mt)
    assert np.isclose(x.mag, 0.)
    x = mag.bar(self.mt, self.nt)
    y = mag.bar(self.mt, self.nt)
    assert np.isclose(abs(x.mag), abs(y.mag))

    x = mag.bar(self.m, self.n)
    y = mag.bar(self.n, self.m)
    assert -1 <= x.mag <= 1.
    assert -1 <= y.mag <= 1.
    assert np.isclose(abs(x.mag), abs(y.mag))

    with pytest.raises(ValueError):
      mag.bar(self.x, self.y)

  def test_mcrossentropy (self):

    x = mag.mcrossentropy(MagT64(-float('Inf')), MagT64(float('Inf')))
    assert np.isinf(x)

    x = mag.mcrossentropy(MagP64(0.), MagP64(0.))
    assert np.isclose(x, np.log(2))

    with pytest.warns(RuntimeWarning):
      x = mag.mcrossentropy(MagP64(0.), MagP64(1.))
      assert np.isnan(x)

    with pytest.warns(RuntimeWarning):
      x = mag.mcrossentropy(MagP64(1.), MagP64(1.))
      assert np.isnan(x)

    x = mag.mcrossentropy(MagT64(0.), MagT64(0.))
    y = mag.mcrossentropy(MagT64(1.), MagT64(0.))
    assert np.isclose(x, y)

    x = mag.mcrossentropy(MagT64(float('Inf')), MagT64(float('Inf')))
    assert np.isclose(x, 0.)

    with pytest.raises(ValueError):
      mag.mcrossentropy(self.x, self.y)

  def test_logZ (self):

    with pytest.raises(ValueError):
      mag.logZ(self.m, self.x)

    with pytest.raises(ValueError):
      mag.logZ(self.m, [self.x])

    with pytest.raises(ValueError):
      mag.logZ(self.mt, self.x)

    with pytest.raises(ValueError):
      mag.logZ(self.mt, [self.x])

    with pytest.raises(ValueError):
      mag.logZ(self.x, self.y)


    u = [MagP64(1.) for _ in range(5)]
    u0 = MagP64(1.)

    x = mag.logZ(u0, u)
    assert np.isclose(x, 0.)

    u = [MagP64(-1.) for _ in range(5)]
    u0 = MagP64(-1.)

    x = mag.logZ(u0, u)
    assert np.isclose(x, 0.)

    u = [MagT64(1.) for _ in range(5)]
    u0 = MagT64(1.)

    x = mag.logZ(u0, u)
    assert np.isclose(x, mag.lr(6))

    u.append(MagT64(-1.))
    x = mag.logZ(u0, u)
    assert np.isinf(x)



  def test_auxmix (self):

    x = mag.auxmix(MagT64(0.), self.x, self.y)
    assert x.mag == 0.

    with pytest.raises(ValueError):
      mag.auxmix(self.m, self.y, self.x)


    x = mag.auxmix(self.mt, 0., 0.)
    assert np.isclose(x.mag, 0.)

    with pytest.warns(RuntimeWarning):
      x = mag.auxmix(self.mt, float('Inf'), 0.)
      assert np.isnan(x.mag)

    with pytest.warns(RuntimeWarning):
      x = mag.auxmix(self.mt, 0., float('Inf'))
      assert np.isnan(x.mag)

    x = mag.auxmix(MagT64(float('Inf')), 0., 0.)
    assert np.isclose(x.mag, 0.)

    x = mag.auxmix(MagT64(float('Inf')), float('Inf'), 0.)
    assert np.isclose(x.mag, .5 * mag.lr(0.))

    x = mag.auxmix(MagT64(float('Inf')), -float('Inf'), 0.)
    assert np.isclose(x.mag, -x.mInf)

    x = mag.auxmix(MagT64(float('Inf')), 0., float('Inf'))
    assert np.isclose(x.mag, -.5 * mag.lr(0.))

    x = mag.auxmix(MagT64(float('Inf')), 0., -float('Inf'))
    assert np.isclose(x.mag, x.mInf)

    x = mag.auxmix(MagT64(float('Inf')), float('Inf'), float('Inf'))
    assert np.isclose(x.mag, 0)

    x = mag.auxmix(MagT64(float('Inf')), -float('Inf'), float('Inf'))
    assert np.isclose(x.mag, -x.mInf)

    x = mag.auxmix(MagT64(float('Inf')), float('Inf'), -float('Inf'))
    assert np.isclose(x.mag, x.mInf)


  def test_erfmix (self):

    with pytest.raises(ValueError):
      mag.erfmix(self.x, self.y, self.x)

    with pytest.raises(ValueError):
      mag.erfmix(self.m, self.m, self.m)

    with pytest.raises(ValueError):
      mag.erfmix(self.mt, self.mt, self.mt)

    x = mag.erfmix(self.mt, self.x, self.x)
    assert np.isclose(x.mag, mag.auxmix(self.mt, self.x, self.x).mag)

    x = mag.erfmix(MagT64(0.), self.x, self.x)
    assert np.isclose(x.mag, 0.)

    x = mag.erfmix(MagP64(0.), self.x, self.x)
    assert np.isclose(x.mag, 0.)

    x = mag.erfmix(self.m, 0., 0.)
    assert np.isclose(x.mag, 0.)


  def test_exactmix (self):

    with pytest.raises(ValueError):
      mag.exactmix(self.m, [self.x], [self.m])

    with pytest.raises(ValueError):
      mag.exactmix(self.mt, [self.x], [self.mt])

    with pytest.raises(ValueError):
      mag.exactmix(self.x, self.y, self.x)

    x = mag.exactmix(self.mt, self.mt, self.mt)
    assert np.isclose(x.mag, mag.auxmix(self.mt, self.mt.mag, self.mt.mag).mag)

    x = mag.exactmix(MagT64(0.), self.mt, self.mt)
    assert np.isclose(x.mag, 0.)

    x = mag.exactmix(MagP64(0.), self.m, self.m)
    assert np.isclose(x.mag, 0.)

    x = mag.exactmix(self.m, self.m, self.m)
    assert np.isclose(x.mag, 0.)
