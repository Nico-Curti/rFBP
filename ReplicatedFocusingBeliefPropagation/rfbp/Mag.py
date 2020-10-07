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
    Base Mag object.
    Abstract data type for magnetization objects.
    The base class implements the base functions for the magnetization algebra.
    Each member can be overrided by a specialization.

    Parameters
    ----------
      x : float
        Value to magnetify

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation.rfbp.Mag import BaseMag
    >>>
    >>> x = np.random.uniform(low=0., high=1)
    >>> m = BaseMag(x)
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
    '''
    The value of the magnetization (it could be equal or processed according to the specialization function)

    Returns
    -------
      x : float
        The value of the magnetization
    '''
    raise NotImplementedError

  @property
  def magformat (self):
    '''
    The name of the specialization used

    Returns
    -------
      name : str
        Name of the specialization
    '''
    raise NotImplementedError

  @_require_mag
  def __mod__ (self, m):
    raise NotImplementedError

  @_require_mag
  def __add__ (self, m):
    '''
    Sum of mags (overload operator +)

    Parameters
    ----------
      m : mag-like object
        A mag data type is required.

    Returns
    -------
      mag : BaseMag
        The result of the operation
    '''
    return self.__class__(self.mag + m.mag)

  def __truediv__ (self, x):
    '''
    Overload operator /

    Parameters
    ----------
      x : float
        This function takes any type of object
    '''
    return self.__class__(self.mag / x)

  def __mul__ (self, x):
    '''
    Overload operator * between mag and number

    Parameters
    ----------
      x : float
        This function takes any type of object

    Returns
    -------
      res : float
        The result of the operation
    '''
    return self.mag * x

  @_require_mag
  def __xor__ (self, m):
    raise NotImplementedError

  @_require_mag
  def __sub__ (self, m):
    '''
    Overload operator - between mags

    Parameters
    ----------
      m : mag-like object
        A mag data type is required.

    Returns
    -------
      x : float
        The result of the operation

    Notes
    -----
    .. note::
      In the operation are involved the "values" of the magnetizations
    '''
    return self.value - m.value

  def __neg__ (self):
    '''
    Overload operator -mag

    Returns
    -------
      mag : BaseMag
        The result of the operation
    '''
    return self.__class__(-self.mag)

  @_require_mag
  def __eq__ (self, m):
    '''
    Check mag equality

    Parameters
    ----------
      m : mag-like object
        A mag data type is required.

    Returns
    -------
      res : bool
        The result of the operation
    '''
    return self.mag == m.mag

  @_require_mag
  def __ne__ (self, m):
    '''
    Check mag difference

    Parameters
    ----------
      m : mag-like object
        A mag data type is required.

    Returns
    -------
      res : bool
        The result of the operation
    '''
    return self.mag != m.mag

  def __repr__ (self):
    '''
    Object representation
    '''
    class_name = self.__class__.__qualname__
    return '{0}(x={1})'.format(class_name, self.mag)
