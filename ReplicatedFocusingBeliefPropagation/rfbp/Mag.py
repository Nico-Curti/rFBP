#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

from functools import wraps

__author__  = ['Nico Curti', "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class BaseMag (object):

  def __init__ (self, x):
    '''
    Base Mag object

    Parameters
    ----------
      x : float
        Value to magnetify
    '''

    self.mag = x

  def _require_mag (func):
    '''
    Check if the function takes mag type in input
    '''
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
    '''
    Sum of mags (overload operator +)
    '''
    return self.__class__(self.mag + m.mag)

  def __truediv__ (self, x):
    '''
    Overload operator /
    '''
    return self.__class__(self.mag / x)

  def __mul__ (self, x):
    '''
    Overload operator * between mag and number
    '''
    return self.mag * x

  @_require_mag
  def __xor__ (self, m):
    raise NotImplementedError

  @_require_mag
  def __sub__ (self, m):
    '''
    Overload operator - between mags
    '''
    return self.value - m.value

  def __neg__ (self):
    '''
    Overload operator -mag
    '''
    return self.__class__(-self.mag)

  @_require_mag
  def __eq__ (self, m):
    '''
    Check mag equality
    '''
    return self.mag == m.mag

  @_require_mag
  def __ne__ (self, m):
    '''
    Check mag difference
    '''
    return self.mag != m.mag

  def __repr__ (self):
    '''
    Object representation
    '''
    class_name = self.__class__.__qualname__
    return '{0}(x={1})'.format(class_name, self.mag)
