#!/usr/bin/env python

import io
import os
import platform
import numpy as np
from Cython.Distutils import build_ext

try:
  from setuptools import setup, Extension, find_packages
except ImportError:
  from distutils.core import setup, Extension, find_packages

from distutils.sysconfig import customize_compiler

# Package meta-data.
NAME = 'ReplicatedFocusingBeliefPropagation'
DESCRIPTION = 'Replicated Focusing Belief Propagation algorithm.'
URL = 'https://github.com/Nico-Curti/rFBP'
EMAIL = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']
AUTHOR = ['Nico Curti', "Daniele Dall'Olio"]
REQUIRES_PYTHON = '>=2.7'
VERSION = None
KEYWORDS = "belief-propagation deep-neural-networks spin-glass"

cpp_compiler = platform.python_compiler()

# What packages are required for this module to be executed?
def get_requires():
  with open('requirements.txt', 'r') as f:
    requirements = f.read()
  return list(filter(lambda x: x != '', requirements.split()))

class my_build_ext(build_ext):
  def build_extensions(self):
    customize_compiler(self.compiler)
    try:
      if 'GCC' in cpp_compiler or 'CLANG' in cpp_compiler:
        self.compiler.compiler_so.remove("-Wstrict-prototypes")
    except (AttributeError, ValueError):
      pass
    build_ext.build_extensions(self)

def read_description():
  try:
    with open('README.md') as r:
      description = '\n'
      description += r.read()
  except Exception:
    return ''

here = os.path.abspath(os.path.dirname(__file__))

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


# Import the README and use it as the long-description.
# Note: this will only work if 'README.md' is present in your MANIFEST.in file!
try:
  with io.open(os.path.join(here, 'README.md'), encoding='utf-8') as f:
    long_description = '\n' + f.read()
except FileNotFoundError:
  long_description = DESCRIPTION

# Load the package's __version__.py module as a dictionary.
about = {}
if not VERSION:
  with open(os.path.join(here, 'ReplicatedFocusingBeliefPropagation', '__version__.py')) as f:
    exec(f.read(), about)
else:
  about['__version__'] = VERSION

if 'GCC' in cpp_compiler or 'CLANG' in cpp_compiler:
  cpp_compiler_args = ['-std=c++17', '-g0', '-fopenmp']
elif 'MSC' in cpp_compiler:
  cpp_compiler_args = ['/std:c++17', '/openmp']
else:
  print('Unknown c++ compiler arg', file=sys.stderr)
  cpp_compiler_args = []
extra_compile_args = cpp_compiler_args + ['-DSTATS', '-DNDEBUG', '-DVERBOSE', '-DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION']

# Where the magic happens:
setup(
  name                = NAME,
  version             = about['__version__'],
  description         = DESCRIPTION,
  long_description    = long_description,
  long_description_content_type = 'text/markdown',
  author              = AUTHOR,
  author_email        = EMAIL,
  maintainer          = AUTHOR,
  maintainer_email    = EMAIL,
  python_requires     = REQUIRES_PYTHON,
  install_requires    = get_requires(),
  url                 = URL,
  download_url        = URL,
  keywords            = KEYWORDS,
  packages            = find_packages(include=['ReplicatedFocusingBeliefPropagation', 'ReplicatedFocusingBeliefPropagation.*'], exclude=('test',)),
  include_package_data=True,
  platforms           = 'any',
  classifiers=[
    # Trove classifiers
    # Full list: https://pypi.python.org/pypi?%3Aaction=list_classifiers
    #'License :: OSI Approved :: GPL License',
    'Programming Language :: Python',
    'Programming Language :: Python :: 3',
    'Programming Language :: Python :: 3.6',
    'Programming Language :: Python :: Implementation :: CPython',
    'Programming Language :: Python :: Implementation :: PyPy'
  ],
  license             = 'MIT',
  cmdclass            = {'build_ext': my_build_ext},
  ext_modules = [
      Extension(name='.'.join(['lib', 'ReplicatedFocusingBeliefPropagation', 'rFBP']),
                sources=rfbp_sources,
                include_dirs=[
                    '.',
                    os.path.join(os.getcwd(), 'ReplicatedFocusingBeliefPropagation', 'include'),
                    os.path.join(os.getcwd(), 'include'),
                    os.path.join(os.getcwd(), 'scorer', 'scorer', 'include'),
                    np.get_include()
                ],
                libraries=rfbp_lib,
                library_dirs=[
                              os.path.join(os.getcwd(), 'lib'),
                              os.path.join('usr', 'lib'),
                              os.path.join('usr', 'local', 'lib'),
                ],  # path to .a or .so file(s)
                extra_compile_args = extra_compile_args,
                extra_link_args = ['-fopenmp'],
                language='c++',
                ),
              ],
)
