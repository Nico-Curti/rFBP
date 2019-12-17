#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import os
import pytest
from ReplicatedFocusingBeliefPropagation import _check_string

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

def test_check_string ():

  a = 'this_is_a_string'
  b = _check_string(a, False)

  assert isinstance(b, bytes)
  assert b == _check_string(b, False)

  d = os.path.dirname(__file__)
  a = os.path.join(d, '..', 'misc.py')
  b = _check_string(a)


  with pytest.raises(TypeError):
    x = 3.14
    y = _check_string(x)
    print(y)

  with pytest.raises(FileNotFoundError):
    a = os.path.join(d, '..', 'misc_.py')
    b = _check_string(a)
    print(b)
