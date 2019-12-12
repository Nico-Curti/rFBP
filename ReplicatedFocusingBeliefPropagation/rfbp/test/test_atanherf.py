#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import numpy as np
from scipy.special import erf

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


def atanherf (x):
  '''
  Compute atanh(erf) for general values of x

  Parameters
  ----------
    x : float
      Input variable

  Returns
  -------
    atanh(erf(x)) : float
      atanh(erf(x)) for any value of x
  '''
  ax = abs(x)

  if ax <= 2.:
    return np.arctanh(erf(x))

  # elif ax <= 15.:
  #   return np.sign(x) * _atanherf_interp(ax)

  else:
    return _atanherf_largex(x)


def _atanherf_largex (x):
  '''
  Approximate atanh(erf) for large values of x
  '''
  x2 = x * x
  ix = 1. / x2

  def _evalpoly (var):
    '''
    Polynomial approximation of coefficient
    '''
    coef = (1., -1.25, 3.0833333333333335,
            -11.03125, 51.0125, -287.5260416666667, 1906.689732142857,
            -14527.3759765625, 125008.12543402778, -1199006.6259765625)
    return sum((j*var**i for i, j in enumerate(coef)))

  return np.sign(x) * (2. * np.log(abs(x)) + np.log(4 * np.pi) + 2. * x2 + ix * _evalpoly(ix) ) * .25
  return np.sign(x) * (np.log(x2 * 4 * np.pi) + 2. * x2 + ix * _evalpoly(ix) ) * .25


def _atanherf_interp (x):
  '''
  Compute atanh(erf) using spline interpolation
  '''
  pass

def test_atanherf ():
  '''
  Tests:
    - if approximated formula is "equal" to the std one for large x
  '''

  np.random.seed(42)

  for _ in range(10):

    x = np.random.uniform(low=4, high=5)
    assert np.isclose(np.arctanh(erf(x)), _atanherf_largex(x), rtol=1e-4, atol=1e-4)



if __name__ == '__main__':

  test_atanherf()

  exit(0)
