#!/usr/bin/env python
# -*- coding: utf-8 -*-

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

from .build import read_version

VERSION = read_version('./CMakeLists.txt')

__version__ = '.'.join(map(str, VERSION))
