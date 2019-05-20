# distutils: language = c++
# cython: language_level=2

cdef extern from "misc.hpp":
  To ** double_pointers_for_cython[Ti,To]( Ti * input_pointer, const int &n, const int &m)
