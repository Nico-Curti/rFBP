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
  '''
  log1p for magnetizations

  Parameters
  ----------
    x : float
      Input variable

  Returns
  -------
    res : float
      value computed as log1p(exp(-2*abs(x)))

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>> x = np.random.uniform(low=0., high=1.)
  >>> assert mag.lr(x) >= 0.
  '''
  return np.log1p(np.exp(-2 * np.abs(x)))

@singledispatch
def sign0 (x):
  '''
  Sign operation valid also for magnetizations

  Parameters
  ----------
    x : float
      Input variable

  Returns
  -------
    res : float
      sign evaluated as 1 - 2*signbit(x)

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>> x = np.random.uniform(low=0., high=1.)
  >>> m = MagP64(x)
  >>> assert mag.sign0(x) in (-1, 1)
  >>> assert mag.sign0(-x) == 1
  >>> assert mag.sign0(m) in (-1, 1)
  '''
  return 1 if np.sign(x) else -1

@sign0.register(BaseMag)
def _ (x):
  return 1 if np.sign(x.mag) else -1

def zeros (x):
  '''
  Fill array of magnetizations with zeros

  Parameters
  ----------
    x : array-like
      Input array or list of Mag objects

  Returns
  -------
    None

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>> x = np.random.uniform(low=0., high=1.)
  >>> mags = [MagP64(_) for _ in range(10)]
  >>> mag.zeros(mags)
  >>> assert all((i.mag == 0 for i in mags))
  >>>
  >>> l = 3.14
  >>> mag.zeros(3.14)
    ValueError('zeros takes an iterable object in input')
  >>>
  >>> l = [MagP64(3.14), x]
  >>> mag.zeros(l)
    ValueError('Incompatible type found. x must be an iterable of Mags')
  '''

  if not isinstance(x, Iterable):
    raise ValueError('zeros takes an iterable object in input')

  if not all(isinstance(i, BaseMag) for i in x):
    raise ValueError('Incompatible type found. x must be an iterable of Mags')

  for i in x:
    i.mag = 0.

@singledispatch
def zero (x):
  '''
  Set magnetization to zero

  Parameters
  ----------
    x : Mag object
      Input variable

  Returns
  -------
    None

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>> x = np.random.uniform(low=0., high=1.)
  >>> m = MagP64(3.14)
  >>> mag.zero(m)
  >>>
  >>> assert m.mag == 0.
  >>> assert m.value == 0.
  >>> mag.zero(x)
    ValueError('Incompatible type found. x must be a Mag')
  '''
  raise ValueError('Incompatible type found. x must be a Mag')

@zero.register(BaseMag)
def _ (x):
  x.mag = 0.

@singledispatch
def mabs (x):
  '''
  Abs for magnetization objects

  Parameters
  ----------
    x : Mag object
      Input variable

  Returns
  -------
    abs : float
      The absolute value of the input

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>> x = np.random.uniform(low=0., high=1.)
  >>> assert mag.mabs(MagP64(x)) >= 0.
  >>> assert mag.mabs(MagP64(-x)) >= 0.
  >>> assert mag.mabs(MagP64(-x)) == mag.mabs(MagP64(x))
  >>>
  >>> mag.mabs(x)
    ValueError('Incompatible type found. x must be a Mag')
  '''
  raise ValueError('Incompatible type found. x must be a Mag')

@mabs.register(BaseMag)
def _ (x):
  return np.abs(x.mag)

def copysign (x, y):
  '''
  Flip magnetization sign if necessary

  Parameters
  ----------
    x : Mag object
      Input variable to check
    y : float
      Varibale from which copy the sign

  Returns
  -------
    m : Mag object
      The corrected mag object

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>> x = np.random.uniform(low=0., high=1.)
  >>> m = MagP64(np.random.uniform(low=0., high=1.))
  >>>
  >>> x = mag.copysign(m, x)
  >>> assert x.mag == m.mag
  >>>
  >>> x = mag.copysign(-m, x)
  >>> assert x.mag == m.mag
  >>>
  >>> x = mag.copysign(m, -x)
  >>> assert x.mag == -m.mag
  >>>
  >>> x = mag.copysign(-m, -x)
  >>> assert x.mag == -m.mag
  '''
  _check_mag(x)
  return -x if np.sign(x.mag) != np.sign(y) else x

@singledispatch
def arrow (m, x):
  '''
  Arrow operator of original code

  Parameters
  ----------
    x : Mag object
      Input variable
    y : float
      Input variable

  Returns
  -------
    m : Mag object
      The result of the operator

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>> x = np.random.uniform(low=0., high=1.)
  >>> m = MagP64(np.random.uniform(low=0., high=1.))
  >>>
  >>> x = mag.arrow(m, x)
  >>> assert isinstance(x, MagP64)
  >>>
  >>>
  >>> y = np.random.uniform(low=0., high=1.)
  >>> mag.arrow(x, y)
    ValueError('m must be MagP64 or MagT64')

  Notes
  -----
  .. note::
    The computation of the arrow operator is different from MagP64 and MagT64.

    - In MagP64 the computation is equivalent to

        mtanh(x * arctanh(m)).

    - In MagT64 the computation is equivalent to

        mtanh(x * m)

  '''
  raise ValueError('m must be MagP64 or MagT64')

@arrow.register(MagP64)
def _ (m, x):
  return MagP64.mtanh(x * np.arctanh(m.mag))

@arrow.register(MagT64)
def _ (m, x):
  return MagT64.mtanh(x * m.mag)

def logmag2p (x):
  '''
  Log operation for magnetization objects

  Parameters
  ----------
    x : Mag object
      Input variable

  Returns
  -------
    m : Mag object
      The result of the operation

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>>
  >>> x = mag.logmag2p(MagP64(0.))
  >>> assert np.isclose(x, np.log(.5))
  >>>
  >>> x = mag.logmag2p(MagT64(-1.))
  >>> assert np.isinf(x)
  '''
  return np.log( (1 + x.mag) * .5)

@singledispatch
def damp (newx, oldx, l):
  '''
  Update magnetization

  Parameters
  ----------
    newx : Mag object
      Update magnetization value
    oldx : Mag object
      Old magnetization value
    l : float
      Scale factor

  Returns
  -------
    m : Mag object
      The result of the update computed as newx * (1 - l) + oldx * l

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>>
  >>> x = np.random.uniform(low=0., high=1.)
  >>> m = MagP64(np.random.uniform(low=0., high=1.))
  >>> n = MagP64(np.random.uniform(low=0., high=1.))
  >>>
  >>> mx = mag.damp(m, n, x)
  >>> my = mag.damp(n, m, 1. - x)
  >>> assert np.isclose(mx.mag, my.mag)
  >>> assert np.isclose(mx.value,m y.value)
  >>>
  >>> mx = mag.damp(m, n, 0.)
  >>> assert np.isclose(mx.mag, m.mag)
  '''
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
  '''
  Diff of magnetizations

  Parameters
  ----------
    m1 : Mag object
      Input variable
    m2 : Mag object
      Input variable

  Returns
  -------
    m : Mag object
      The result of the diff as (m1 - m2)/(1 - m1 * m2) clamped to [-1, 1] if MagP else m1 - m2

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>>
  >>> m = MagP64(np.random.uniform(low=0., high=1.))
  >>> n = MagP64(np.random.uniform(low=0., high=1.))
  >>>
  >>> mx = mag.bar(m, n)
  >>> my = mag.bar(n, m)
  >>> assert -1 <= mx.mag <= 1.
  >>> assert -1 <= my.mag <= 1.
  >>> assert np.isclose(abs(mx.mag), abs(my.mag))
  '''
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
def log1pxy (x, y):
  '''
  Compute the log1p for the combination of the magnetizations

  Parameters
  ----------
    x : Mag object
      The input variable
    y : Mag object
      The input variable

  Returns
  -------
    res : float
      The result of the operation

  Notes
  -----
  .. note::
    The computation of the function is different from MagP64 and MagT64.

    - In MagP64 the computation is equivalent to

        np.log((1. + (x.mag * y.mag)) * 0.5)

    - In MagT64 the computation takes care of possible number overflows

  '''
  raise ValueError('Both magnetizations must be the same')

@log1pxy.register(MagP64)
def _ (x, y):
  _check_mag(y, MagP64)
  return np.log((1. + (x.mag * y.mag)) * .5)

@log1pxy.register(MagT64)
def _ (x, y):
  _check_mag(y, MagT64)

  ax = x.mag
  ay = y.mag

  if not np.isinf(ax) and not np.isinf(ay):
    return abs(ax + ay) - abs(ax) - abs(ay) + lr(ax + ay) - lr(ax) - lr(ay)
  elif np.isinf(ax) and not np.isinf(ay):
    return np.sign(ax) * ay - abs(ay) - lr(ay)
  elif not np.isinf(ax) and np.isinf(ay):
    return np.sign(ay) * ax - abs(ax) - lr(ax)
  elif np.sign(ax) == np.sign(ay):
    return 0
  else:
    return float('Inf')


@singledispatch
def mcrossentropy (x, y):
  '''
  Compute the crossentropy score for magnetization objects

  Parameters
  ----------
    x : Mag objects
      Input variable
    y : Mag objects
      Input variable

  Returns
  -------
    res : float
      The resulting crossentropy score

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import MagT64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>>
  >>> x = mag.mcrossentropy(MagT64(-float('Inf')), MagT64(float('Inf')))
  >>> assert np.isinf(x)
  >>>
  >>> x = mag.mcrossentropy(MagP64(0.), MagP64(0.))
  >>> assert np.isclose(x, np.log(2))
  >>>
  >>> x = mag.mcrossentropy(MagP64(0.), MagP64(1.))
  >>> assert np.isnan(x)
  >>>
  >>> x = mag.mcrossentropy(MagP64(1.), MagP64(1.))
  >>> assert np.isnan(x)
  >>>
  >>> x = mag.mcrossentropy(MagT64(0.), MagT64(0.))
  >>> y = mag.mcrossentropy(MagT64(1.), MagT64(0.))
  >>> assert np.isclose(x, y)
  >>>
  >>> x = mag.mcrossentropy(MagT64(float('Inf')), MagT64(float('Inf')))
  >>> assert np.isclose(x, 0.)
  >>>
  >>> x = np.random.uniform(low=0., high=1.)
  >>> y = np.random.uniform(low=0., high=1.)
  >>> mag.mcrossentropy(x, y)
    ValueError('Both magnetizations must be the same')

  Notes
  -----
  .. note::
    The computation of the mcrossentropy is different from MagP64 and MagT64.

    - In MagP64 the computation is equivalent to

        -x.mag * np.arctanh(y.mag) - np.log1p(- y.mag**2) * .5 + np.log(2)

    - In MagT64 the computation is equivalent to

        -abs(y.mag) * (sign0(y.mag) * x.value - 1.) + lr(y.mag)

  '''
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
  '''
  '''
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
  '''
  Combine three MagT64 magnetizations

  Parameters
  ----------
    H : MagT64 object
      Input variable
    ap : float
      Input variable
    am : float
      Input variable

  Returns
  -------
    m : MagT64 object
      The result of the mix

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>>
  >>> x = np.random.uniform(low=0., high=1.)
  >>> y = np.random.uniform(low=0., high=1.)
  >>>
  >>> mx = mag.auxmix(MagT64(0.), x, y)
  >>> assert mx.mag == 0.
  >>>
  >>> m = MagP64(np.random.uniform(low=0., high=1.))
  >>> mag.auxmix(m, y, x)
    ValueError('H must be a MagT64 magnetization type')

  Notes
  -----
  .. note::
    This operation is valid only for MagT64 variables up to now
  '''
  raise ValueError('H must be a MagT64 magnetization type')

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
  '''
  Combine three magnetizations with the erf

  Parameters
  ----------
    H : Mag object
      Input variable
    mp : float
      Input variable
    mm : float
      Input variable

  Returns
  -------
    m : Mag object
      The result of the mix

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>>
  >>> x = np.random.uniform(low=0., high=1.)
  >>> m = MagP64(np.random.uniform(low=0., high=1.))
  >>>
  >>> mx = mag.erfmix(MagP64(0.), x, x)
  >>> assert np.isclose(mx.mag, 0.)
  >>>
  >>> mx = mag.erfmix(m, 0., 0.)
  >>> assert np.isclose(mx.mag, 0.)

  Notes
  -----
  .. note::
    The computation of the erfmix is different from MagP64 and MagT64.

    - In MagP64 the computation is equivalent to

        H.mag * (erf(mp) - erf(mm)) / (2. + H.mag * (erf(mp) + erf(mm)))

    - In MagT64 the computation is equivalent to

        auxmix(H, atanherf(mp), atanherf(mm))

  '''
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
  '''
  Combine exactly three magnetizations

  Parameters
  ----------
    H : Mag object
      Input variable
    pp : Mag object
      Input variable
    pm : Mag object
      Input variable

  Returns
  -------
    m : Mag object
      The result of the mix

  Example
  -------
  >>> import numpy as np
  >>> from ReplicatedFocusingBeliefPropagation import MagP64
  >>> from ReplicatedFocusingBeliefPropagation import magnetization as mag
  >>>
  >>> x = np.random.uniform(low=0., high=1.)
  >>> m = MagP64(np.random.uniform(low=0., high=1.))
  >>>
  >>> mag.exactmix(m, [x], [m])
    ValueError('Input variables must magnetizations (MagP64 or MagT64)')
  >>>
  >>> mx = mag.exactmix(MagP64(0.), m, m)
  >>> assert np.isclose(x.mag, 0.)
  >>>
  >>> mx = mag.exactmix(m, m, m)
  >>> assert np.isclose(mx.mag, 0.)

  Notes
  -----
  .. note::
    The computation of the exactmix is different from MagP64 and MagT64.

    - In MagP64 the computation is equivalent to

        (pp.mag - pm.mag) * H.mag / (2. + (pp.mag + pm.mag) * H.mag)

    - In MagT64 the computation is equivalent to

        auxmix(H, pp.mag, pm.mag)

  '''
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
