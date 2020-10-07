#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import os
import sys
import warnings
from contextlib import contextmanager

__author__  = ['Nico Curti', "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

def _check_string (String, exist=True):
  '''
  Check if the input string is already encoded for c++ compatibility

  Parameters
  ----------
    String : string or bytes
      String to convert / verify

    exist : bool (default = True)
      If the string identify a filename check if it exist

  Returns
  -------
    str: Encoded string (utf-8)

  Example
  -------
  >>> a = 'this_is_a_string'
  >>> b = _check_string(a, False)
  >>> assert isinstance(b, bytes)
    True
  >>> assert b == _check_string(b, False)
    True

  Notes
  -----
  .. note::
    The strings must be converted to bytes for c++ function compatibility!
  '''

  if not isinstance(String, str) and not isinstance(String, bytes):
    raise TypeError('{} must be in string format'.format(String))

  if exist:
    if not os.path.isfile(String):
      raise FileNotFoundError('Could not open or find the data file. Given: {}'.format(String))

  return String.encode('utf-8') if isinstance(String, str) else String


@contextmanager
def redirect_stdout (verbose):
  '''
  Redirect output stdout from cython wrap to devnull or not.
  This function works ONLY for cython wrap functions!!
  If you want to redirect python prints you can use something like

  Parameters
  ----------
    verbose: bool
      Switch if turn on/off the output redirection

  Example
  -------
  >>> from io import StringIO
  >>> import contextlib
  >>> temp_stdout = StringIO()
  >>> foo = lambda : print('hello world!')
  >>> with contextlib.redirect_stdout(temp_stdout):
  >>>   foo()

  '''

  try:
    # Temporary fix for the IPython console.
    # The current version of the redirect_stdout does not support the
    # redirection using IPython.
    # Error: "ValueError: write to closed file"
    # TODO: fix this issue with some workaround for the devnull redirection in IPython
    __IPYTHON__
    warnings.warn('The current version does not allow to redirect the stdout using an IPython console.', RuntimeWarning)
    verbose = True

  except NameError:
    pass

  if verbose:
    try:
      yield
    finally:
      return

  to = os.devnull

  fd = sys.stdout.fileno()

  # assert that Python and C stdio write using the same file descriptor
  # assert libc.fileno(ctypes.c_void_p.in_dll(libc, "stdout")) == fd == 1

  def _redirect_stdout (to):
    sys.stdout.close()              # + implicit flush()
    os.dup2(to.fileno(), fd)        # fd writes to 'to' file
    sys.stdout = os.fdopen(fd, 'w') # Python writes to fd

  with os.fdopen(os.dup(fd), 'w') as old_stdout:
    with open(to, 'w') as file:
      _redirect_stdout(to=file)
    try:
      yield # allow code to be run with the redirected stdout
    finally:
      _redirect_stdout(to=old_stdout) # restore stdout.
                                      # buffering and flags such as
                                      # CLOEXEC may be different

def get_int_size ():
  '''
  Get the correct integer size according to your operative system.
  This function is just an utility for the cython wrap of the objects.
  '''

  if sys.platform.lower() == 'win32':
    return 'int32'

  else:
    return 'int64'