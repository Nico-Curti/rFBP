#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import numpy as np
from collections.abc import Iterable
from .Mag import BaseMag
from .MagP64 import MagP64
from .MagT64 import MagT64

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

def _check_mag (x):
  if not isinstance(x, BaseMag):
    raise InputError('Incompatible type found. x must be a Mag')

def lr (x):
  return np.log1p(np.exp(-2 * abs(x)))

def sign0 (x):
  if isinstance(x, BaseMag):
    return 1 if np.sign(x.mag) else -1
  else:
    return 1 if np.sign(x) else -1

def zeros (x):
  if not isinstance(x, Iterable):
    raise ValueError('zeros takes an iterable object in input')

  if not all(isinstance(i, BaseMag) for i in x):
    raise InputError('Incompatible type found. x must be an iterable of Mags')

  for i in x:
    i.mag = 0.

@BaseMag._require_mag
def zero (x):
  x.mag = 0.

@BaseMag._require_mag
def abs (x):
  return abs(x.mag)

def copysign (x, y):
  _check_mag(x)
  return -x if np.sign(x.mag) != np.sign(y) else x

def arrow (m, x):
  _check_mag(m)

  if isinstance(m, MagP64):
    return mtanh(x * np.arctanh(m.mag), mag='MagP64')

  elif isinstance(m, MagT64):
    return mtanh(x * m.mag, mag='MagT64')

  else:
    raise NotImplementedError

@BaseMag._require_mag
def logmag2p (x):
  return np.log( (1 + x.mag) * .5)

def convert (x, mag):
  if mag == 'MagP64':
    return MagP64(x)
  elif mag == 'MagT64':
    return MagT64(np.clip(np.arctanh(x), -30., 30.))
  elif mag == 'none':
    return x.value
  else:
    raise ValueError('mag must be MagP64 or MagT64')

def couple (x1, x2, mag):
  if mag == 'MagP64':
    return MagP64( (x1 - x2)/(x1 + x2) )
  elif mag == 'MagT64':
    return convert(np.log(x1 / x2) * .5, mag='MagT64')
  else:
    raise ValueError('mag must be MagP64 or MagT64')

def damp (newx, oldx, l):

  _check_mag(newx)
  _check_mag(oldx)

  if mag == 'MagP64':
    return MagP64( newx.mag * (1. - l) + oldx.mag * l )
  elif mag == 'MagT64':
    return convert( newx.value * (1. - l) + oldx.value * l, mag='MagT64')
  else:
    raise ValueError('mag must be MagP64 or MagT64')

def mtanh (x, mag):
  if mag == 'MagP64':
    return MagP64( np.tanh(x) )
  elif mag == 'MagT64':
    return MagT64(x)
  else:
    raise ValueError('mag must be MagP64 or MagT64')

def merf (x, mag):
  if mag == 'MagP64':
    return MagP64( erf(x) )
  elif mag == 'MagT64':
    return MagT64( atanherf(x) )
  else:
    raise ValueError('mag must be MagP64 or MagT64')



