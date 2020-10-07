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
    MagP64 specialization.
    The `MagT64` exactly follows all theoretical equations with no further approximation, which necessarily causes slower executions.
    In practice it computes the tanh of each numerical value.

    Parameters
    ----------
      x : float
        Value to magnetify

      mInf : float (default = 30.)
        Clip value

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation.rfbp.Mag import MagT64
    >>>
    >>> x = np.random.uniform(low=0., high=1)
    >>> m = MagT64(x)
    '''

    super(MagT64, self).__init__(x)
    self.mInf = mInf

  @property
  def value(self):
    '''
    Return the mag value

    Returns
    -------
      x : float
        In MagT64 the value is equal to the tanh(x)

    Example
    -------
    >>> from ReplicatedFocusingBeliefPropagation import MagT64
    >>> x = np.random.uniform(low=0, high=1.)
    >>> m = MagT64(x)
    >>> assert np.isclose(m.mag, x)
    >>> assert np.isclose(m.value, np.tanh(x))
    '''
    return np.tanh(self.mag)

  @property
  def magformat (self):
    '''
    Return the mag description

    Returns
    -------
      tanh : str
        The MagT64 type corresponds to a tanh operation

    Example
    -------
    >>> from ReplicatedFocusingBeliefPropagation import MagT64
    >>> m = MagT64(3.14)
    >>> m.magformat
      'tanh'
    '''
    return 'tanh'

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
      m : MagT64
        Convert any-number to a MagT64 type

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation import MagT64
    >>>
    >>> x = np.random.uniform(low=0., high=10)
    >>> mx = MagT64.convert(x)
    >>> assert -30. <= mx.mag <= 30.
    >>> assert np.isclose(mx.mag, np.arctanh(x))
    >>> assert np.isclose(mx.value, x)
    '''
    return MagT64(np.clip(np.arctanh(x), -30., 30.))

  @staticmethod
  def couple (x1, x2):
    '''
    Combine two mags

    Parameters
    ----------
      x1 : float
        The first element of the operation
      x2 : float
        The second element of the operation

    Returns
    -------
      x : float
        Mags combination as np.log(x1 / x2) * .5

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation import MagT64
    >>>
    >>> x = np.random.uniform(low=0., high=10)
    >>> y = np.random.uniform(low=0., high=10)
    >>> mx = MagT64.couple(x, y)
    >>> my = MagT64.couple(y, x)
    >>> assert np.isclose(abs(mx.mag), abs(my.mag))
    >>> assert np.isclose(abs(mx.value), abs(my.value))
    '''
    return MagT64.convert(np.log(x1 / x2) * .5)

  @staticmethod
  def mtanh (x):
    '''
    Perform tanh on magnetization value (MagT64(x) in this case)

    Parameters
    ----------
      x : float
        The input value

    Returns
    -------
      m : MagT64
        The MagT64 version of the tanh(x)

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation import MagT64
    >>>
    >>> x = np.random.uniform(low=0., high=10)
    >>> mx = MagT64.mtanh(x)
    >>> assert np.isclose(mx.mag, x)
    >>> assert np.isclose(mx.value, np.tanh(x))
    '''
    return MagT64(x)

  @staticmethod
  def merf (x):
    '''
    Perform erf on magnetization value (MagT64(erf(x)) in this case)

    Parameters
    ----------
      x : float
        The input value

    Returns
    -------
      m : MagT64
        The MagT64 version of the erf(x)

    Example
    -------
    >>> import numpy as np
    >>> from scipy.special import erf
    >>> from ReplicatedFocusingBeliefPropagation import MagT64
    >>>
    >>> x = np.random.uniform(low=0., high=10)
    >>> mx = MagT64.merf(x)
    >>> assert np.isclose(mx.mag, np.arctanh(erf(x)))
    '''
    return MagT64( atanherf(x) )

  @BaseMag._require_mag
  def __mod__ (self, m):
    '''
    In this case the mod operation corresponds to a sum of mags

    Parameters
    ----------
      m : MagT64
        The input value

    Returns
    -------
      m : MagT64
        The MagT64 of the operation between the two mags.
        The mod operation corresponds to self.mag + m.mag

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation import MagT64
    >>>
    >>> x = np.random.uniform(low=0., high=10)
    >>> y = np.random.uniform(low=0., high=1)
    >>> m1 = MagT64(x)
    >>> m2 = MagT64(y)
    >>>
    >>> mx = m1 % m2
    >>> my = m2 % m1
    >>> assert np.isclose(mx.mag, my.mag)
    >>> assert np.isclose(mx.value, my.value)
    >>>
    >>> null = MagT64(0.)
    >>> mx = m1 % null
    >>> assert np.isclose(mx.mag, m1.mag)
    >>> assert np.isclose(mx.value, m1.value)
    '''
    return self.__class__(self.mag + m.mag)

  @BaseMag._require_mag
  def __xor__ (self, m):
    '''
    Mag product

    Parameters
    ----------
      m : MagT64
        The input value

    Returns
    -------
      m : MagT64
        The product of mags

    Example
    -------
    >>> import numpy as np
    >>> from ReplicatedFocusingBeliefPropagation import MagT64
    >>>
    >>> x = np.random.uniform(low=0., high=10)
    >>> y = np.random.uniform(low=0., high=1)
    >>> m1 = MagT64(x)
    >>> m2 = MagT64(y)
    >>>
    >>>
    >>> mx = m1 ^ m2
    >>> my = m2 ^ m1
    >>> assert np.isclose(mx.mag, my.mag)
    >>> assert np.isclose(mx.value, my.value)
    >>>
    >>> mx = (-m1) ^ (-m2)
    >>> my = (-m2) ^ (-m1)
    >>> assert not np.isclose(mx.mag, my.mag)
    >>> assert not np.isclose(mx.value, my.value)
    >>>
    >>> null = MagT64(0.)
    >>> mx = m1 ^ null
    >>> assert np.isclose(mx.mag, 0.)
    >>> assert np.isclose(mx.value, 0.)
    >>>
    >>> mx = m1 ^ MagT64(float('inf'))
    >>> assert np.isclose(mx.mag, m1.mag)
    >>> assert np.isclose(mx.value, m1.value)
    '''
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
