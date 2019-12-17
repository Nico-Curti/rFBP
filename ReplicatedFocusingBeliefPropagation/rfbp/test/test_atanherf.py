#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import numpy as np
from scipy.special import erf
from ReplicatedFocusingBeliefPropagation import atanherf

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

def test_atanherf ():
  '''
  Tests:
    - if approximated formula is "equal" to the std one for large x
  '''

  np.random.seed(42)

  for _ in range(10):

    x = np.random.uniform(low=4, high=5)
    assert np.isclose(np.arctanh(erf(x)), atanherf(x), rtol=1e-4, atol=1e-4)

  for _ in range(10):

    x = np.random.uniform(low=0., high=1.)
    assert np.isclose(np.arctanh(erf(x)), atanherf(x), rtol=1e-4, atol=1e-4)


