#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import numpy as np
from scipy.special import erf
from ReplicatedFocusingBeliefPropagation.rfbp.Mag import BaseMag

__author__  = ['Nico Curti', "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class MagP64 (BaseMag):

  def __init__ (self, x):
    '''
    MagP64 specialization.
    The `MagP64` type allows fast executions with inexact outcomes by neglecting all `tanh` operations.

    Parameters
    ----------
      x : float
        Value to magnetify

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation.rfbp.Mag import MagP64
    >>>
    >>> x = np.random.uniform(low=0., high=1)
    >>> m = MagP64(x)
    '''
    super(MagP64, self).__init__(x)

  @property
  def value (self):
    '''
    Return the mag value

    Returns
    -------
      x : float
        In MagP64 the value is equal to the magnetization since the tanh operation is neglected

    Example
    -------
    >>> from ReplicatedFocusingBeliefPropagation import MagP64
    >>> x = np.random.uniform(low=0, high=1.)
    >>> m = MagP64(x)
    >>> assert np.isclose(m.mag, x)
    >>> assert np.isclose(m.value, x)
    '''
    return self.mag

  @property
  def magformat (self):
    '''
    Return the mag description

    Returns
    -------
      plain : str
        The MagP64 type corresponds to a plain operation

    Example
    -------
    >>> from ReplicatedFocusingBeliefPropagation import MagP64
    >>> m = MagP64(3.14)
    >>> m.magformat
      'plain'
    '''
    return 'plain'

  @staticmethod
  def convert (x):
    '''
    Convert a float to a mag value (as a constructor)

    Parameters
    ----------
      x : float
        The number to convert

    Returns
    -------
      m : MagP64
        Convert any-number to a MagP64 type

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation import MagP64
    >>>
    >>> x = np.random.uniform(low=0., high=10)
    >>> m1 = MagP64.convert(x)
    >>> m2 = MagP64(x)
    >>> assert m1.mag == m2.mag
    >>> assert m1.value == m2.value
    '''
    return MagP64(x)

  @staticmethod
  def couple (x1, x2):
    '''
    Combine two mags as diff / sum

    Parameters
    ----------
      x1 : float
        The first element of the operation
      x2 : float
        The second element of the operation

    Returns
    -------
      x : float
        In MagP64 the value is equal to the magnetization since the tanh operation is neglected

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation import MagP64
    >>>
    >>> x = np.random.uniform(low=0., high=10)
    >>> y = np.random.uniform(low=0., high=10)
    >>> mx = MagP64.couple(x, y)
    >>> my = MagP64.couple(y, x)
    >>> assert np.isclose(abs(mx.mag), abs(my.mag))
    >>> assert np.isclose(abs(mx.value), abs(my.value))
    '''
    return MagP64( (x1 - x2)/(x1 + x2) )

  @staticmethod
  def mtanh (x):
    '''
    Perform tanh on magnetization value (MagP64(tanh(x)) in this case)

    Parameters
    ----------
      x : float
        The input value

    Returns
    -------
      m : MagP64
        The MagP64 version of the tanh(x)

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation import MagP64
    >>>
    >>> x = np.random.uniform(low=0., high=10)
    >>> mx = MagP64.mtanh(x)
    >>> assert 0 <= mx.mag <= 1
    >>> assert np.isclose(mx.mag, np.tanh(x))
    '''
    return MagP64(np.tanh(x))

  @staticmethod
  def merf (x):
    '''
    Perform erf on magnetization value (MagP64(erf(x)) in this case)

    Parameters
    ----------
      x : float
        The input value

    Returns
    -------
      m : MagP64
        The MagP64 version of the erf(x)

    Example
    -------
    >>> import numpy as np
    >>> from scipy.special import erf
    >>> from ReplicatedFocusingBeliefPropagation import MagP64
    >>>
    >>> x = np.random.uniform(low=0., high=10)
    >>> mx = MagP64.merf(x)
    >>> assert 0 <= mx.mag <= 1
    >>> assert np.isclose(mx.mag, erf(x))
    '''
    return MagP64(erf(x))

  @BaseMag._require_mag
  def __mod__ (self, m):
    '''
    Clip value in [-1, 1].

    Parameters
    ----------
      m : MagP64
        The input value

    Returns
    -------
      m : MagP64
        The MagP64 of the operation between the two mags.
        The clip operation is computed as np.clip( (self.mag + m.mag) / (1. + self.mag * m.mag), -1., 1.)

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation import MagP64
    >>> x = np.random.uniform(low=0., high=10)
    >>> y = np.random.uniform(low=0., high=10)
    >>> m1 = MagP64(x)
    >>> m2 = MagP64(y)
    >>> mx = m1 % m2
    >>> my = m2 % m1
    >>> assert np.isclose(mx.mag, my.mag)
    >>> assert np.isclose(mx.value, my.value)
    >>> assert -1. <= mx.mag <= 1.
    >>> assert -1. <= my.mag <= 1.
    >>> assert -1. <= mx.value <= 1.
    >>> assert -1. <= my.value <= 1.
    '''
    return self.__class__(np.clip( (self.mag + m.mag) / (1. + self.mag * m.mag), -1., 1.))

  @BaseMag._require_mag
  def __xor__ (self, m):
    '''
    Mag product

    Parameters
    ----------
      m : MagP64
        The input value

    Returns
    -------
      m : MagP64
        The product of mags

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation import MagP64
    >>> x = np.random.uniform(low=0., high=10)
    >>> y = np.random.uniform(low=0., high=10)
    >>> m1 = MagP64(x)
    >>> m2 = MagP64(y)
    >>> mx = m1 ^ m2
    >>> my = m2 ^ m1
    >>> assert np.isclose(mx.mag, my.mag)
    >>> assert np.isclose(mx.value, my.value)
    '''
    return self.__class__(self.mag * m.mag)

