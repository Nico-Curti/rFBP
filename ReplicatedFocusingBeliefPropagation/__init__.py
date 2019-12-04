#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import os
import warnings

try:

  from .rfbp.ReplicatedFocusingBeliefPropagation import ReplicatedFocusingBeliefPropagation
  from .rfbp.ReplicatedFocusingBeliefPropagation import NTH
  from .rfbp.Patterns import Pattern
  from .rfbp.FocusingProtocol import Focusing_Protocol
  from lib.ReplicatedFocusingBeliefPropagation.rFBP import Mag

except ImportError:
  pass

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

ATANHERF_INTERP_FILE = os.path.join(os.path.dirname(__file__),
                                    '..', 'data', 'atanherf_interp.max_16.step_0.0001.first_1.dat')

if not os.path.isfile(ATANHERF_INTERP_FILE):
  warnings.warn("Atanherf file not found. It will be downloaded before continue")
  import sys
  sys.path.append(os.path.join(os.path.dirname(__file__),
                               '..', 'scripts'))
  print(ATANHERF_INTERP_FILE)
  from download_atanherf import get_atanherf
  get_atanherf('179OcORFBvcUGzGHBEVU8lUGLS70c1ZI_')
