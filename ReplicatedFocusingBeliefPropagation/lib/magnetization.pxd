# distutils: language = c++
# cython: language_level=2

from MagP64 cimport MagP64
from MagT64 cimport MagT64

ctypedef fused mag_t:

  MagP64
  MagT64
