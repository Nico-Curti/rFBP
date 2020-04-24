# distutils: language = c++
# cython: language_level=2

from cython.operator cimport dereference as deref

from Patterns cimport Patterns
from misc cimport double_pointers_for_cython


cdef class _Patterns:

  def __init__ (self, double[::1] X=None, long int[::1] y=None, filename=b'', int binary=0, delimiter=b'\t', long int M=-1, long int N=-1):

    cdef int cpp_M, cpp_N

    if X is not None and y is not None:

      # convert matrix to Pattern obj

      cpp_N, cpp_M = (M, N)

      self.thisptr.reset(new Patterns(double_pointers_for_cython[double, double](&X[0], N, M), &y[0], N, M))

    elif filename:

      # load Pattern from file

      self.thisptr.reset(new Patterns(filename, binary, delimiter))

    elif M > 0 and N > 0:

      # random pattern
      self.thisptr.reset(new Patterns(M, N))

    else:

      raise ValueError('Wrong input variable provided. Please give (X, y) or a valid filename')

    self.Nrow = deref(self.thisptr).Nrow
    self.Ncol = deref(self.thisptr).Ncol

  @property
  def labels (self):
    '''
    Return the label array
    '''
    return [deref(self.thisptr).output[i] for i in range(self.Nrow)]

  @property
  def data (self):
    '''
    Return the data matrix
    '''
    data = [[deref(self.thisptr).input[i][j] for j in range(self.Ncol)] for i in range(self.Nrow)]
    return data

  def __repr__ (self):
    '''
    Object representation
    '''
    class_name = self.__class__.__name__
    return '<{} Class (M: {} N: {} >)'.format(class_name, self.Nrow, self.Ncol)
