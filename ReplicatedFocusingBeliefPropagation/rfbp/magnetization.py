#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import numpy as np
import operator as op
from functools import reduce
from scipy.special import erf
from functools import singledispatch
from collections.abc import Iterable
from ReplicatedFocusingBeliefPropagation.rfbp.Mag import BaseMag
from ReplicatedFocusingBeliefPropagation.rfbp.MagP64 import MagP64
from ReplicatedFocusingBeliefPropagation.rfbp.MagT64 import MagT64
from ReplicatedFocusingBeliefPropagation.rfbp.atanherf import atanherf

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

def _check_mag (x, cls=BaseMag):
  if not isinstance(x, cls):
    raise ValueError('Incompatible type found. x must be a Mag')

def lr (x):
  return np.log1p(np.exp(-2 * np.abs(x)))

@singledispatch
def sign0 (x):
  return 1 if np.sign(x) else -1

@sign0.register(BaseMag)
def _ (x):
  return 1 if np.sign(x.mag) else -1

def zeros (x):
  if not isinstance(x, Iterable):
    raise ValueError('zeros takes an iterable object in input')

  if not all(isinstance(i, BaseMag) for i in x):
    raise ValueError('Incompatible type found. x must be an iterable of Mags')

  for i in x:
    i.mag = 0.

@singledispatch
def zero (x):
  raise ValueError('Incompatible type found. x must be a Mag')

@zero.register(BaseMag)
def _ (x):
  x.mag = 0.

@singledispatch
def mabs (x):
  raise ValueError('Incompatible type found. x must be a Mag')

@mabs.register(BaseMag)
def _ (x):
  return np.abs(x.mag)

def copysign (x, y):
  _check_mag(x)
  return -x if np.sign(x.mag) != np.sign(y) else x

@singledispatch
def arrow (m, x):
  raise ValueError('m must be MagP64 or MagT64')

@arrow.register(MagP64)
def _ (m, x):
  return MagP64.mtanh(x * np.arctanh(m.mag))

@arrow.register(MagT64)
def _ (m, x):
  return MagT64.mtanh(x * m.mag)

def logmag2p (x):
  return np.log( (1 + x.mag) * .5)

@singledispatch
def damp (newx, oldx, l):
  raise ValueError('Both magnetizations must be the same')

@damp.register(MagP64)
def _ (newx, oldx, l):
  _check_mag(oldx, cls=MagP64)
  return MagP64( newx.mag * (1. - l) + oldx.mag * l )

@damp.register(MagT64)
def _ (newx, oldx, l):
  return MagT64.convert( newx.value * (1. - l) + oldx.value * l)

@singledispatch
def bar (m1, m2):
  raise ValueError('Both magnetizations must be the same')

@bar.register(MagP64)
def _ (m1, m2):
  _check_mag(m2, cls=MagP64)
  return MagP64( np.clip((m1.mag - m2.mag) / (1. - m1.mag * m2.mag), -1., 1.) )

@bar.register(MagT64)
def _ (m1, m2):
  _check_mag(m2, cls=MagT64)
  return MagT64(m1.mag - m2.mag)

@singledispatch
def mcrossentropy (x, y):
  raise ValueError('Both magnetizations must be the same')

@mcrossentropy.register(MagP64)
def _ (x, y):
  _check_mag(y, MagP64)
  return -x.mag * np.arctanh(y.mag) - np.log1p(- y.mag**2) * .5 + np.log(2)

@mcrossentropy.register(MagT64)
def _ (x, y):
  _check_mag(y, MagT64)

  if not np.isinf(y.mag):
    return -abs(y.mag) * (sign0(y.mag) * x.value - 1.) + lr(y.mag)

  elif np.sign(x.value) != np.sign(y.mag):
    return float('inf')

  else:
    return 0.

@singledispatch
def logZ (u0, u):
  raise ValueError('Both magnetizations must be the same')

@logZ.register(MagP64)
def _ (u0, u):
  if not isinstance(u, Iterable):
    raise ValueError('zeros takes an iterable object in input')

  if not all(isinstance(i, MagP64) for i in u):
    raise ValueError('Incompatible type found. x must be an iterable of Mags')

  prod = lambda args: reduce(op.mul, args, 1)

  zkip = (1. + u0.mag) * .5 * prod( ((1. + x.mag) * .5 for x in u) )
  zkim = (1. - u0.mag) * .5 * prod( ((1. - x.mag) * .5 for x in u) )

  return np.log( zkip + zkim )

@logZ.register(MagT64)
def _ (u0, u):
  if not isinstance(u, Iterable):
    raise ValueError('zeros takes an iterable object in input')

  if not all(isinstance(i, MagT64) for i in u):
    raise ValueError('Incompatible type found. x must be an iterable of Mags')

  s1 = sum((i.mag for i in u if not np.isinf(i.mag)))
  s1 += 0. if np.isinf(u0.mag) else u0.mag

  s2 = sum((abs(i.mag) for i in u if not np.isinf(i.mag)))
  s2 += 0. if np.isinf(u0.mag) else u0.mag

  s3 = sum((lr(i.mag) for i in u if not np.isinf(i.mag)))
  s3 += 0. if np.isinf(u0.mag) else lr(u0.mag)

  hasinf = np.sign(u0.mag) if np.isinf(u0.mag) else 0.
  for i in u:
    if hasinf == 0.:
      hasinf = np.sign(i.mag)
    elif hasinf != np.sign(i.mag):
      return -float('Inf')

  return np.abs(s1) - s2 + lr(s1) - s3


@singledispatch
def auxmix (H, ap, am):
  raise ValueError('H must be a magnetization (MagP64 or MagT64)')

@auxmix.register(MagT64)
def _ (H, ap, am):

  if H.mag == 0.:
    return MagT64(0.)

  else:

    xH = H.mag + ap
    xh = H.mag + am
    a_ap = abs(ap)
    a_am = abs(am)

    if np.isinf(H.mag):

      if not np.isinf(ap) and not np.isinf(am):
        t1 = np.sign(H.mag) * (ap - am) - a_ap + a_am
        t2 = -lr(ap) + lr(am)

      elif np.isinf(ap) and not np.isinf(am):
        t1 = -np.sign(H.mag) * am + a_am if np.sign(ap) == np.sign(H.mag) else -2. * H.mInf
        t2 = lr(am) if np.sign(ap) == np.sign(H.mag) else 0.

      elif not np.isinf(ap) and np.isinf(am):
        t1 = np.sign(H.mag) * ap + a_ap if np.sign(am) == np.sign(H.mag) else 2. * H.mInf
        t2 = -lr(ap) if np.sign(am) == np.sign(H.mag) else 0.

      else:
        t2 = 0.

        if (np.sign(ap) == np.sign(H.mag) and np.sign(am) == np.sign(H.mag) ) or (np.sign(ap) != np.sign(H.mag) and np.sign(am) != np.sign(H.mag)):
          t1 = 0.
        elif np.sign(ap) == np.sign(H.mag):
          t1 = 2. * H.mInf
        else:
          t1 = -2. * H.mInf

    else:
      t1 = 0
      if not np.isinf(ap):
        t1 += abs(xH) - a_ap
      if not np.isinf(am):
        t1 -= abs(xh) - a_am

      t2 = np.log( (np.exp(2. * abs(xH)) + 1.) * (np.exp(2. * abs(am)) + 1.) / ((np.exp(2. * abs(ap)) + 1.) * (np.exp(2. * abs(xh)) + 1.))) - 2. * abs(xH) + 2. * abs(ap) + 2. * abs(xh) - 2. * abs(am)

  return MagT64((t1 + t2) * .5)

@singledispatch
def erfmix (H, mp, mm):
  raise ValueError('H must be a magnetization (MagP64 or MagT64)')

@erfmix.register(MagP64)
def _ (H, mp, mm):
  if all((not isinstance(i, BaseMag) for i in (mp, mm))):
    arg = H.mag * (erf(mp) - erf(mm)) / (2. + H.mag * (erf(mp) + erf(mm)))
    return MagP64(arg)
  else:
    raise ValueError('Input variables must be all not magnetization types')


@erfmix.register(MagT64)
def _ (H, mp, mm):
  if all((not isinstance(i, BaseMag) for i in (mp, mm))):
    return auxmix(H, atanherf(mp), atanherf(mm))
  else:
    raise ValueError('Input variables must be all not magnetization types')

@singledispatch
def exactmix (H, pp, pm):
  raise ValueError('Input variables must magnetizations (MagP64 or MagT64)')

@exactmix.register(MagP64)
def _ (H, pp, pm):
  if all((isinstance(i, MagP64) for i in (pp, pm))):
    arg = (pp.mag - pm.mag) * H.mag / (2. + (pp.mag + pm.mag) * H.mag)
    return MagP64(arg)

  else:
    raise ValueError('Input variables must be all the same magnetization (MagP64 or MagT64)')

@exactmix.register(MagT64)
def _ (H, pp, pm):
  if all((isinstance(i, MagT64) for i in (pp, pm))):
    return auxmix(H, pp.mag, pm.mag)

  else:
    raise ValueError('Input variables must be all the same magnetization (MagP64 or MagT64)')
