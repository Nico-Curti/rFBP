#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import json
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

from distutils import sysconfig
from Cython.Distutils import build_ext
from distutils.sysconfig import customize_compiler
from distutils.command.sdist import sdist as _sdist


def get_requires (requirements_filename):
  '''
  What packages are required for this module to be executed?

  Parameters
  ----------
    requirements_filename : str
      filename of requirements (e.g requirements.txt)

  Returns
  -------
    requirements : list
      list of required packages
  '''
  with open(requirements_filename, 'r') as fp:
    requirements = fp.read()

  return list(filter(lambda x: x != '', requirements.split()))


def get_ext_filename_without_platform_suffix (filename):
    name, ext = os.path.splitext(filename)
    ext_suffix = sysconfig.get_config_var('EXT_SUFFIX')

    if ext_suffix == ext:
      return filename

    ext_suffix = ext_suffix.replace(ext, '')
    idx = name.find(ext_suffix)

    if idx == -1:
      return filename
    else:
      return name[:idx] + ext


class rfbp_build_ext (build_ext):
  '''
  Custom build type
  '''

  def get_ext_filename (self, ext_name):

    if platform.system() == 'Windows':
      # The default EXT_SUFFIX of windows include the PEP 3149 tags of compiled modules
      # In this case I rewrite a custom version of the original distutils.command.build_ext.get_ext_filename function
      ext_path = ext_name.split('.')
      ext_suffix = '.pyd'
      filename = os.path.join(*ext_path) + ext_suffix
    else:
      filename = super().get_ext_filename(ext_name)

    return get_ext_filename_without_platform_suffix(filename)

  def build_extensions (self):

    customize_compiler(self.compiler)

    try:
      self.compiler.compiler_so.remove('-Wstrict-prototypes')

    except (AttributeError, ValueError):
      pass

    build_ext.build_extensions(self)


class sdist(_sdist):
  def run(self):
    self.run_command("build_ext")
    _sdist.run(self)


def read_description (readme_filename):
  '''
  Description package from filename

  Parameters
  ----------
    readme_filename : str
      filename with readme information (e.g README.md)

  Returns
  -------
    description : str
      str with description
  '''

  try:

    with open(readme_filename, 'r') as fp:
      description = '\n'
      description += fp.read()

  except FileNotFoundError:
    return ''


def read_dependencies_build (dependencies_filename):
  '''
  Read the json of dependencies
  '''
  with open(dependencies_filename, 'r') as fp:
    dependecies = json.load(fp)

  return dependecies


here = os.path.abspath(os.path.dirname(__file__)).replace('\\', '/')

# Package meta-data.
NAME = 'ReplicatedFocusingBeliefPropagation'
DESCRIPTION = 'Replicated Focusing Belief Propagation algorithm.'
EMAIL = 'nico.curti2@unibo.it, daniele.dallolio@studio.unibo.it'
AUTHOR = "Nico Curti, Daniele Dall'Olio"
REQUIRES_PYTHON = '>=3.5'
VERSION = None
KEYWORDS = "belief-propagation deep-neural-networks spin-glass"

CPP_COMPILER = platform.python_compiler()
README_FILENAME = os.path.join(here, 'README.md')
REQUIREMENTS_FILENAME = os.path.join(here, 'requirements.txt')
DEPENDENCIES_FILENAME = os.path.join(here, 'ReplicatedFocusingBeliefPropagation', 'dependencies.json')
VERSION_FILENAME = os.path.join(here, 'ReplicatedFocusingBeliefPropagation', '__version__.py')

ENABLE_OMP = False
BUILD_SCORER = False

# Import the README and use it as the long-description.
# Note: this will only work if 'README.md' is present in your MANIFEST.in file!
try:
  LONG_DESCRIPTION = read_description(README_FILENAME)

except FileNotFoundError:
  LONG_DESCRIPTION = DESCRIPTION


current_python = sys.executable.split('/bin')[0]
numpy_dir = current_python + '/lib/python{0}.{1}/site-packages/numpy/core/include'.format(sys.version_info.major, sys.version_info.minor)
if os.path.isdir(numpy_dir):
  os.environ['CFLAGS'] = '-I' + numpy_dir


# Load the package's __version__.py module as a dictionary.
about = {}
if not VERSION:
  with open(VERSION_FILENAME) as fp:
    exec(fp.read(), about)

else:
  about['__version__'] = VERSION

# parse version variables and add them to command line as definitions
Version = about['__version__'].split('.')
URL = 'https://github.com/Nico-Curti/rFBP/archive/v{}.tar.gz'.format(about['__version__'])

# Read dependecies graph
dependencies = read_dependencies_build(DEPENDENCIES_FILENAME)

# Set compiler variables
define_args = [ '-DMAJOR={}'.format(Version[0]),
                '-DMINOR={}'.format(Version[1]),
                '-DREVISION={}'.format(Version[2]),
                '-DSTATS',
                '-DNDEBUG',
                '-DVERBOSE',
                '-DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION',
                '-DPWD="{}"'.format(here)
              ]


if 'GCC' in CPP_COMPILER or 'Clang' in CPP_COMPILER:
  cpp_compiler_args = ['-std=c++1z', '-std=gnu++1z', '-g0']

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

  try:

    compiler, compiler_version = CPP_COMPILER.split()

  except ValueError:

    compiler, compiler_version = (CPP_COMPILER, '0')

  if compiler == 'GCC' and BUILD_SCORER:
    BUILD_SCORER = True if int(compiler_version[0]) > 4 else False

  if ENABLE_OMP and compiler == 'GCC':
    linker_args = ['-fopenmp']
  else:
    linker_args = []

  if compiler == 'Clang':
    ENABLE_OMP = False


elif 'MSC' in CPP_COMPILER:
  cpp_compiler_args = ['/std:c++latest', '/Ox', '/Wall', '/W3']
  compile_args = []

  if ENABLE_OMP:
    linker_args = ['/openmp']
  else:
    linker_args = []

else:
  raise ValueError('Unknown c++ compiler arg')


if BUILD_SCORER:
  scorer_include = [os.path.join(os.getcwd(), 'scorer', 'include'),
                    os.path.join(os.getcwd(), 'scorer', 'scorer', 'include'),]
else:
  scorer_include = []


whole_compiler_args = sum([cpp_compiler_args, compile_args, define_args, linker_args], [])

cmdclass = {'build_ext': rfbp_build_ext,
            'sdist': sdist}


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
  setup_requires                = [# Setuptools 18.0 properly handles Cython extensions.
                                   'setuptools>=18.0',
                                   'cython',
                                   'numpy'],
  packages                      = find_packages(include=['ReplicatedFocusingBeliefPropagation',
                                                         'ReplicatedFocusingBeliefPropagation.*'],
                                                exclude=('test', 'example')),
  include_package_data          = True,
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
  cmdclass                      = cmdclass,
  ext_modules                   = [
                                    Extension(name='.'.join(['ReplicatedFocusingBeliefPropagation', 'lib', name]),
                                              sources=values['sources'],
                                              include_dirs=sum([values['include_dirs'], [np.get_include()]], []),
                                              libraries=values['libraries'],
                                              library_dirs=[
                                                            os.path.join(here, 'lib'),
                                                            os.path.join('usr', 'lib'),
                                                            os.path.join('usr', 'local', 'lib'),
                                              ],  # path to .a or .so file(s)
                                              extra_compile_args = whole_compiler_args,
                                              extra_link_args = linker_args,
                                              language='c++'
                                              )

                                    for name, values in dependencies.items() ],
)
