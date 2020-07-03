# distutils: language = c++
# cython: language_level=2

from libcpp.memory cimport unique_ptr

cdef extern from "misc.hpp" nogil:

  To ** double_pointers_for_cython[Ti,To]( Ti * input_pointer, const long int & n, const long int & m);

  type * unique_pointer_to_pointer[type] (const unique_ptr[type[]] & src, const long int & size);

  unique_ptr[type[]] pointer_to_unique_pointer[type] (type * src, const long int & size);