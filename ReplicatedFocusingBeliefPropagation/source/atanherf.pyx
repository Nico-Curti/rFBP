# distutils: language = c++
# cython: language_level=2

from atanherf cimport atanherf_largex
from atanherf cimport atanherf_interp
from atanherf cimport evalpoly
from atanherf cimport atanherf

def _atanherf_largex (double x):
  return atanherf_largex(x)

def _atanherf_interp (double x):
  return atanherf_interp(x)

def _evalpoly (double x):
  return evalpoly(x)

def _atanherf (double x):
  return atanherf(x)
