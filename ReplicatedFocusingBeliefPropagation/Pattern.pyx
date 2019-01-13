#!/usr/bin/env python
# distutils: language = c++
# distutils: sources = pattern.cpp
# cython: language_level=3

cimport cython
from libcpp cimport bool
from libcpp.string cimport string
from libcpp.memory cimport unique_ptr
from cython.operator cimport dereference as deref
import pandas as pd

cdef string leave_uninitalized = string()

cdef extern from "pattern.h":
  cppclass Patterns:
    Patterns() except +
    Patterns(string, bool) except +
    Patterns(long int N, long int M) except +

    # Attributes
    long int Nrow, Ncol, Nout
    unique_ptr[long int] output
    double ** input


cdef class Pattern:
  cdef unique_ptr[Patterns] thisptr

  def __cinit__(self, long int N = -1, long int M = -1, string filename = leave_uninitalized, bool binary = False):
    if N != -1 and M != -1 and filename == leave_uninitalized:
      self.thisptr.reset(new Patterns(N, M))
    elif N == -1 and M == -1 and not filename != leave_uninitalized:
      self.thisptr.reset(new Patterns(filename, binary))
    else:
      self.thisptr.reset(new Patterns())

  def __init__(self, N = -1, M = -1, filename = "", bin = False):
    self.Nrow = deref(self.thisptr).Nrow
    self.Ncol = deref(self.thisptr).Ncol

    lbl  = [ (deref(self.thisptr).output.get())[i] for i in range(self.Ncol)]
    data = [[(deref(self.thisptr).input)[i][j] for j in range(self.Ncol)] for i in range(self.Nrow)]

    self.data = pd.DataFrame(data = data, columns=lbl)


  def __repr__(self):
    class_name = self.__class__.__name__
    return '<%s Class (Ninput: '%class_name + str(self.Nrow) + 'Nfeatures: ' + str(self.Ncol) + '>)'

  def __str__(self):
    return self.data

  def __getitem__(self, idx):
    i, j = idx
    return self.data.iloc[i, j]

  def __setitem__(self, idx, value):
    i, j = idx
    self.data[i, j] = value

