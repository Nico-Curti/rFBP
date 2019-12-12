#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import os
import sys

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

def _check_string(_string, exist=True):

  if not isinstance(_string, (str, bytes)):
    raise TypeError('{0} must be in string format'.format(_string))

  if isinstance(_string, bytes):
    _string = _string.decode(sys.getfilesystemencoding())

  if exist and not os.path.isfile(_string):
    raise FileNotFoundError('{0} file not found'.format(_string))

  return _string.encode(sys.getfilesystemencoding())


def test_check_string ():

  a = 'this_is_a_string'
  b = _check_string(a, False)

  if not isinstance(b, bytes):
    raise ValueError

  if not b == _check_string(b, False):
    raise ValueError

  a = 'misc.py'
  b = _check_string(a)

  x = 3.14

  try:
    y = _check_string(x)
    print(y)

  except TypeError:
    pass


if __name__ == '__main__':

  test_check_string()

