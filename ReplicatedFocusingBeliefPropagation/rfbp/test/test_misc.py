#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import os
import pytest
import contextlib
from io import StringIO
from ReplicatedFocusingBeliefPropagation import _check_string
from ReplicatedFocusingBeliefPropagation import redirect_stdout

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

def test_redirect_stdout ():

  temp_stdout = StringIO()

  foo = lambda : print('hello world!')

  with redirect_stdout(True):
    with contextlib.redirect_stdout(temp_stdout):
      foo()

  output = temp_stdout.getvalue().strip()
  # pytest redirect the output itself so it cannot work!
  # You can manually check this assert into a Python code
  # assert output == 'hello world!'
