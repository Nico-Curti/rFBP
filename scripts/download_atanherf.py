#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

from zipfile import ZipFile
import requests
import time
import sys
import os

__author__  = ['Nico Curti', "Daniele Dall'Olio"]
__email__ = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

def download_file_from_google_drive(Id, destination, total_length=230636758):

  url = 'https://docs.google.com/uc?export=download'

  def get_confirm_token(response):
    for key, value in response.cookies.items():
      if key.startswith('download_warning'): return value
    return None

  def save_response_content(response, destination):
    chunk_size = 32768
    with open(destination, 'wb') as fp:
      dl = 0
      start = time.time()
      download = time.time()
      for chunk in response.iter_content(chunk_size):
        dl += len(chunk)
        done = int(50 * dl / total_length)
        length = len(chunk) / 1000000 / (time.time() - download) if time.time() - download else 0
        sys.stdout.write('\r[%s%s] %3d%% (%1.1f Mb/sec) %3.0f sec' % ('o' * done,
                                                                   '-' * (50 - done),
                                                                   dl / total_length * 100,
                                                                   length,
                                                                   time.time() - start)
                                                                  )
        download = time.time()
        sys.stdout.flush()
        if chunk: # filter out keep-alive new chunks
          fp.write(chunk)
    sys.stdout.write('\n')

  session  = requests.Session()
  response = session.get(url, params = {'id' : Id }, stream = True)
  token    = get_confirm_token(response)

  if token:
    params = { 'id' : Id, 'confirm' : token }
    response = session.get(url, params = params, stream = True)

  save_response_content(response, destination)


def get_atanherf(Id):

  size = 4241037
  file = 'atanherf_interp.max_16.step_0.0001.first_1'
  print ('Download {0} file...'.format(file))
  download_file_from_google_drive(Id, './{}.zip'.format(file.lower()), size)

  here = os.path.join(os.path.dirname(__file__))

  print ('Extracting files...', end='')
  with ZipFile('./{}.zip'.format(file.lower())) as zipper:
    zipper.extractall('.')
  print ('[done]')

  try:
    os.makedirs(os.path.join(here, '../data'), exist_ok=True)

  except TypeError: # Python 2 version
    os.makedirs(os.path.join(here, '../data'))

  os.rename('./{}.dat'.format(file.lower()), os.path.join(here, '../data/{}.dat'.format(file.lower())) )

  os.remove('./{}.zip'.format(file.lower()))


if __name__ == '__main__':

  get_atanherf('179OcORFBvcUGzGHBEVU8lUGLS70c1ZI_')
