#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import numpy as np
from ReplicatedFocusingBeliefPropagation import Pattern

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class TestPattern:
  '''
  Test Pattern object and its wrap in Cython
  '''

  def test_null_constructor (self):
    '''
    Tests:
      - is constructed
      - dimensions
      - data
      - labels
    '''

    # constructor
    pt = Pattern()
    print(pt)
    assert pt.pattern is None

    # dimensions
    assert pt.shape == (0, 0)

    # data
    assert pt.data is None

    # labels
    assert pt.labels is None

  def test_constructor (self):
    '''
    Tests:
      - is constructed
      - dimensions
      - data
      - labels
    '''

    n_sample = 10
    n_feature = 20

    data = np.random.choice(a=(-1, 1), p=(.5, .5), size=(n_sample, n_feature))
    labels = np.random.choice(a=(-1, 1), p=(.5, .5), size=(n_sample,))

    pt = Pattern(X=data, y=labels)
    print(pt)

    # constructor
    assert pt.pattern is not None

    # dimensions
    assert pt.shape == (n_sample, n_feature)

    # data
    assert np.allclose(pt.data, data)

    # labels
    assert np.allclose(pt.labels, labels)

  def test_random_pattern (self):
    '''
    Tests:
      - is constructed
      - dimensions
      - data
      - labels
    '''

    n_sample = 10
    n_feature = 20

    # constructed
    pt = Pattern().random(shape=(n_sample, n_feature))
    print(pt)

    # dimensions
    assert pt.shape == (n_sample, n_feature)

    # data
    values = set(np.ravel(pt.data))
    assert 0 < len(values) <= 2
    assert all(v in (-1, 1) for v in values)

    # labels
    values = set(pt.labels)
    assert 0 < len(values) <= 2
    assert all(v in (-1, 1) for v in values)

