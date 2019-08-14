#!/usr/bin/env python
# -*- coding: utf-8 -*-

def read_version(CMakeLists):
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

__package__ = 'version'
__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

VERSION = read_version('./CMakeLists.txt')

__version__ = '.'.join(map(str, VERSION))
