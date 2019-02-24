#!/usr/bin/env python
# distutils: language = c++
# distutils: sources = fprotocol.cpp
# cython: language_level=3

cimport cython
from libcpp.string cimport string
from libcpp.memory cimport unique_ptr
from cython.operator cimport dereference as deref


__package__ = "rFBP_protocol"
__author__  = ["Nico Curti (nico.curit2@unibo.it)", "Daniele Dall'Olio (daniele.dallolio@studio.unibo.it)"]

cdef extern from "fprotocol.h":
  cppclass FocusingProtocol:
    FocusingProtocol() except +
    FocusingProtocol(string prot, long int size) except +

    # Members
    long int Nrep
    unique_ptr[double] gamma, n_rep, beta

    # Attributes
    void StandardReinforcement( double *rho, long int Nrho)
    void StandardReinforcement( double drho)
    void Scoping( double *gr,  double x,  long int ngr)
    void PseudoReinforcement( double *rho,  long int nrho, double x)
    void PseudoReinforcement( double drho, double x)
    void FreeScoping( double **list,  long int nlist)


cdef class FProtocol:
  cdef unique_ptr[FocusingProtocol] thisptr

  cdef public:
    object Nrep, gamma, n_rep, beta

  def __cinit__(self, prot, size):
    self.thisptr.reset(new FocusingProtocol(prot, size))

  def __init__(self, prot, size):
    self.Nrep  = deref(self.thisptr).Nrep
    self.gamma = [(deref(self.thisptr).gamma.get())[i] for i in range(self.Nrep)]
    self.n_rep = [(deref(self.thisptr).n_rep.get())[i] for i in range(self.Nrep)]
    self.beta  = [(deref(self.thisptr).beta.get() )[i] for i in range(self.Nrep)]

  def __repr__(self):
    class_name = self.__class__.__name__
    return '<%s Class>'%(class_name)

  def __str__(self):
    fmt_str  = 'gamma: %s\n'%self.gamma
    fmt_str += 'n_rep: %s\n'%self.n_rep
    fmt_str += 'beta:  %s\n'%self.beta
    return fmt_str

  def __getitem__(self, i):
    return (self.gamma[i], self.n_rep[i], self.beta[i])
