#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import os
import sys

__package__ = "rFBP miscellaneous"
__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

# https://stackoverflow.com/questions/14197009/how-can-i-redirect-print-output-of-a-function-in-python
try:

  import contextlib

  @contextlib.contextmanager
  def stdout_redirect(where):
    sys.stdout = where
    try:
      yield where
    finally:
      sys.stdout = sys.__stdout__

  def redirect(where, func):
    with stdout_redirect(where) as new_stdout:
      func()

except:

  import StringIO

  def stdout_redirect(where, func):
    stdout = sys.stdout
    sys.stdout = StringIO.StringIO()
    func()
    sys.stdout = stdout

  def redirect(where, func):
    stdout_redirect(where, func)




def _check_string(_string, exist=True):

  if not isinstance(_string, str) and not isinstance(_string, bytes):
    raise TypeError('{0} must be in string format'.format(_string))

  if exist: _check_exist(_string)

  if isinstance(_string, str):
    return _string.encode('utf-8')

  return _string

def _check_exist(_file):
  if not os.path.isfile(_file):
    raise ValueError('{0} file not found'.format(_file))
