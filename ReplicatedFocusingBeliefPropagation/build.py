#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

from Cython.Distutils import build_ext
from distutils.sysconfig import customize_compiler

__author__  = ['Nico Curti']
__email__ = ['nico.curit2@unibo.it']


def read_version (CMakeLists):
  '''
  Read version from variables set in CMake file
  '''
  version = []

  with open(CMakeLists, 'r') as fp:

    for row in fp:

      if len(version) == 3: break

      if 'RFBP_MAJOR' in row:
        version.append(row.split('RFBP_MAJOR')[-1])

      elif 'RFBP_MINOR' in row:
        version.append(row.split('RFBP_MINOR')[-1])

      elif 'RFBP_REVISION' in row:
        version.append(row.split('RFBP_REVISION')[-1])

  version = [v.strip().replace(')', '').replace('(', '') for v in version]
  version = map(int, version)

  return tuple(version)


def get_requires (requirements_filename):
  '''
  What packages are required for this module to be executed?
  '''
  with open(requirements_filename, 'r') as fp:
    requirements = fp.read()

  return list(filter(lambda x: x != '', requirements.split()))



class rfbp_build_ext (build_ext):
  '''
  Custom build type
  '''

  def build_extensions (self):

    customize_compiler(self.compiler)

    try:
      self.compiler.compiler_so.remove('-Wstrict-prototypes')

    except (AttributeError, ValueError):
      pass

    build_ext.build_extensions(self)


def read_description (readme_filename):
  '''
  Description package from filename
  '''

  try:

    with open(readme_filename, 'r') as fp:
      description = '\n'
      description += fp.read()

  except Exception:
    return ''

