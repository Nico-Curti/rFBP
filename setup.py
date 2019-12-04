#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function

import os
import sys
import platform
import numpy as np

try:
  from setuptools import setup
  from setuptools import Extension
  from setuptools import find_packages

except ImportError:
  from distutils.core import setup
  from distutils.core import Extension
  from distutils.core import find_packages

from ReplicatedFocusingBeliefPropagation.build import get_requires
from ReplicatedFocusingBeliefPropagation.build import rfbp_build_ext
from ReplicatedFocusingBeliefPropagation.build import read_description

here = os.path.abspath(os.path.dirname(__file__))

# Package meta-data.
NAME = 'ReplicatedFocusingBeliefPropagation'
DESCRIPTION = 'Replicated Focusing Belief Propagation algorithm.'
URL = 'https://github.com/Nico-Curti/rFBP'
EMAIL = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']
AUTHOR = ['Nico Curti', "Daniele Dall'Olio"]
REQUIRES_PYTHON = '>=2.7'
VERSION = None
KEYWORDS = "belief-propagation deep-neural-networks spin-glass"

CPP_COMPILER = platform.python_compiler()
README_FILENAME = os.path.join(here, 'README.md')
REQUIREMENTS_FILENAME = os.path.join(here, 'requirements.txt')
VERSION_FILENAME = os.path.join(here, 'ReplicatedFocusingBeliefPropagation', '__version__.py')

current_python = sys.executable.split('/bin')[0]
numpy_dir = current_python + '/lib/python{}.{}/site-packages/numpy/core/include'.format(sys.version_info.major, sys.version_info.minor)
if os.path.isdir(numpy_dir):
  os.environ['CFLAGS'] = '-I' + numpy_dir

# Import the README and use it as the long-description.
# Note: this will only work if 'README.md' is present in your MANIFEST.in file!
try:
  LONG_DESCRIPTION = read_description(README_FILENAME)

except FileNotFoundError:
  LONG_DESCRIPTION = DESCRIPTION


# Load the package's __version__.py module as a dictionary.
about = {}
if not VERSION:
  with open(VERSION_FILENAME) as fp:
    exec(fp.read(), about)

else:
  about['__version__'] = VERSION

# parse version variables and add them to command line as definitions
Version = about['__version__'].split('.')

if not os.path.isfile(os.path.join(here, 'lib', 'librfbp.so')):
  rfbp_sources = [os.path.join(os.getcwd(), 'ReplicatedFocusingBeliefPropagation', 'source', 'rFBP.pyx'),
                  os.path.join(os.getcwd(), 'src', 'cavity_message.cpp'),
                  os.path.join(os.getcwd(), 'src', 'magnetization.cpp'),
                  os.path.join(os.getcwd(), 'src', 'fprotocol.cpp'),
                  os.path.join(os.getcwd(), 'src', 'atanherf.cpp'),
                  os.path.join(os.getcwd(), 'src', 'pattern.cpp'),
                  os.path.join(os.getcwd(), 'src', 'spline.cpp'),
                  os.path.join(os.getcwd(), 'src', 'utils.cpp'),
                  os.path.join(os.getcwd(), 'src', 'rfbp.cpp')
                  ]
  rfbp_lib = []
else:
  rfbp_sources = [os.path.join(os.getcwd(), 'ReplicatedFocusingBeliefPropagation', 'source', 'rFBP.pyx')
                 ]
  rfbp_lib = ['rfbp']


if 'GCC' in CPP_COMPILER or 'Clang' in CPP_COMPILER:
  cpp_compiler_args = ['-std=c++17', '-g0', '-fopenmp']
  compiler, compiler_version = CPP_COMPILER.split()
  if compiler == 'GCC':
    BUILD_SCORER = True if int(compiler_version[0]) > 4 else False
  if compiler == 'Clang':
    BUILD_SCORER = True

elif 'MSC' in CPP_COMPILER:
  cpp_compiler_args = ['/std:c++17', '/openmp']
  BUILD_SCORER = True
else:
  raise ValueError('Unknown c++ compiler arg')

define_args = [ '-DMAJOR={}'.format(Version[0]),
                '-DMINOR={}'.format(Version[1]),
                '-DREVISION={}'.format(Version[2]),
                '-DSTATS',
                '-DNDEBUG',
                '-DVERBOSE',
                '-DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION',
                '-DPWD="{}"'.format(here)
              ]

compile_args = [ '-Wno-unused-function', # disable unused-function warnings
                 '-Wno-narrowing', # disable narrowing conversion warnings
                  # enable common warnings flags
                 '-Wall',
                 '-Wextra',
                 '-Wno-unused-result',
                 '-Wno-unknown-pragmas',
                 '-Wfatal-errors',
                 '-Wpedantic',
                 '-march=native',
               ]


if BUILD_SCORER:
  scorer_include = [os.path.join(os.getcwd(), 'scorer', 'include'),
                    os.path.join(os.getcwd(), 'scorer', 'scorer', 'include'),]
else:
  scorer_include = []

whole_compiler_args = sum([cpp_compiler_args, compile_args, define_args], [])

# Where the magic happens:
setup(
  name                          = NAME,
  version                       = about['__version__'],
  description                   = DESCRIPTION,
  long_description              = LONG_DESCRIPTION,
  long_description_content_type = 'text/markdown',
  author                        = AUTHOR,
  author_email                  = EMAIL,
  maintainer                    = AUTHOR,
  maintainer_email              = EMAIL,
  python_requires               = REQUIRES_PYTHON,
  install_requires              = get_requires(REQUIREMENTS_FILENAME),
  url                           = URL,
  download_url                  = URL,
  keywords                      = KEYWORDS,
  packages                      = find_packages(include=['ReplicatedFocusingBeliefPropagation',
                                                         'ReplicatedFocusingBeliefPropagation.*'],
                                                exclude=('test', 'example')),
  include_package_data          =True,
  platforms                     = 'any',
  classifiers                   = [
                                    # Full list: https://pypi.python.org/pypi?%3Aaction=list_classifiers
                                    #'License :: OSI Approved :: GPL License',
                                    'Programming Language :: Python',
                                    'Programming Language :: Python :: 3',
                                    'Programming Language :: Python :: 3.6',
                                    'Programming Language :: Python :: Implementation :: CPython',
                                    'Programming Language :: Python :: Implementation :: PyPy'
                                  ],
  license                       = 'MIT',
  cmdclass                      = {'build_ext': rfbp_build_ext},
  ext_modules                   = [
                                    Extension(name='.'.join(['lib', 'ReplicatedFocusingBeliefPropagation', 'rFBP']),
                                              sources=rfbp_sources,
                                              include_dirs=[
                                                  '.',
                                                  os.path.join(os.getcwd(), 'ReplicatedFocusingBeliefPropagation', 'include'),
                                                  os.path.join(os.getcwd(), 'include'),
                                                  *scorer_include,
                                                  np.get_include()
                                              ],
                                              libraries=rfbp_lib,
                                              library_dirs=[
                                                            os.path.join(os.getcwd(), 'lib'),
                                                            os.path.join('usr', 'lib'),
                                                            os.path.join('usr', 'local', 'lib'),
                                              ],  # path to .a or .so file(s)
                                              extra_compile_args = whole_compiler_args,
                                              extra_link_args = ['-fopenmp'],
                                              language='c++',
                                              ),
                                            ],
)
