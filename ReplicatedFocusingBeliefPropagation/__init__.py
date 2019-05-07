#!/usr/bin/env python

import os
import warnings
from lib.ReplicatedFocusingBeliefPropagation.rFBP import Mag
from ReplicatedFocusingBeliefPropagation.source.ReplicatedFocusingBeliefPropagation import ReplicatedFocusingBeliefPropagation, NTH
from ReplicatedFocusingBeliefPropagation.source.Patterns import Pattern
from ReplicatedFocusingBeliefPropagation.source.FocusingProtocol import Focusing_Protocol

__all__ = ["ReplicatedFocusingBeliefPropagation"]

__package__ = "rFBP"
__author__  = ["Nico Curti (nico.curit2@unibo.it)", "Daniele Dall'Olio (daniele.dallolio@studio.unibo.it)"]

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
