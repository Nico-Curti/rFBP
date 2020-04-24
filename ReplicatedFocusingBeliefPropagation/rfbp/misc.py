#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import os
import sys

__author__  = ['Nico Curti', "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

def _check_string (String, exist=True):
  '''
  Check if the input string is already encoded for c++ compatibility

  Parameters
  ----------
  String : string or bytes
    String to convert / verify

  exist : bool (default = True)
    If the string identify a filename check if it exist

  Returns
  -------
  Encoded string (utf-8)

  Notes
  -----
  The strings must be converted to bytes for c++ function compatibility!
  '''

  if not isinstance(String, str) and not isinstance(String, bytes):
    raise TypeError('{} must be in string format'.format(String))

  if exist:
    if not os.path.isfile(String):
      raise FileNotFoundError('Could not open or find the data file. Given: {}'.format(String))

  return String.encode('utf-8') if isinstance(String, str) else String
