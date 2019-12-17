#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

from functools import wraps

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class BaseMag (object):

  def __init__ (self, x):

    self.mag = x

  def _require_mag (func):

    @wraps(func)
    def _ (self, m):
      if isinstance(m, self.__class__):
        return func(self, m)
      else:
        raise NotImplementedError

    return _

  @property
  def value (self):
    raise NotImplementedError

  @property
  def magformat (self):
    raise NotImplementedError

  @_require_mag
  def __mod__ (self, m):
    raise NotImplementedError

  @_require_mag
  def __add__ (self, m):
    return self.__class__(self.mag + m.mag)

  @_require_mag
  def __iadd__ (self, m):
    self.mag += m.mag
    return self

  def __truediv__ (self, x):
    return self.__class__(self.mag / x)

  def __mul__ (self, x):
    return self.mag * x

  @_require_mag
  def __xor__ (self, m):
    raise NotImplementedError

  @_require_mag
  def __sub__ (self, m):
    return self.value - m.value

  def __neg__ (self):
    return self.__class__(-self.mag)

  @_require_mag
  def __eq__ (self, m):
    return self.mag == m.mag

  @_require_mag
  def __ne__ (self, m):
    return self.mag != m.mag

  def __repr__ (self):
    class_name = self.__class__.__qualname__
    return '{0}(x={1})'.format(class_name, self.mag)

