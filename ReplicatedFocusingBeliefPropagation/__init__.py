#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import os
import sys
import warnings
from .__version__ import __version__

from .rfbp.ReplicatedFocusingBeliefPropagation import ReplicatedFocusingBeliefPropagation
from .rfbp.ReplicatedFocusingBeliefPropagation import NTH
from .rfbp.Patterns import Pattern
from .rfbp.FocusingProtocol import Focusing_Protocol
from .rfbp.atanherf import atanherf
from .rfbp.MagP64 import MagP64
from .rfbp.MagT64 import MagT64
from .rfbp import magnetization
from .rfbp.misc import _check_string
from .rfbp.misc import redirect_stdout

__author__  = ['Nico Curti', "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

ATANHERF_INTERP_FILE = os.path.join(os.path.dirname(__file__),
                                    '..', 'data',
                                    'atanherf_interp.max_16.step_0.0001.first_1.dat')

if not os.path.isfile(ATANHERF_INTERP_FILE):

  warnings.warn('Atanherf file not found. It will be downloaded before continue')

  sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'scripts'))

  from download_atanherf import get_atanherf

  get_atanherf('179OcORFBvcUGzGHBEVU8lUGLS70c1ZI_')
