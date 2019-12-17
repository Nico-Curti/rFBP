#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import os
import pytest
import numpy as np
from sklearn.exceptions import NotFittedError
from ReplicatedFocusingBeliefPropagation import MagP64
from ReplicatedFocusingBeliefPropagation import MagT64
from ReplicatedFocusingBeliefPropagation import Pattern
from ReplicatedFocusingBeliefPropagation import ReplicatedFocusingBeliefPropagation as rFBP

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class TestRFBP:
  '''
  Test rFBP model
  '''
  np.random.seed(42)

  def test_constructor (self):

    pattern = Pattern().random(shape=(20, 101))

    params = { 'mag' : MagT64,
               'hidden' : 3,
               'max_iter' : 1000,
               'seed' : 135,
               'damping' : .5,
               'accuracy' : ('accurate', 'exact'),
               'randfact' : .1,
               'epsil' : .5,
               'protocol' : 'pseudo_reinforcement',
               'size' : 101,
               'nth' : 2
              }

    rfbp = rFBP(**params)
    assert params == rfbp.get_params()
    print(rfbp)

    # test wrong number of accuracy
    params['accuracy'] = ('none')
    with pytest.raises(TypeError):
      rfbp = rFBP(**params)
      assert params == rfbp.get_params()

    # test wrong accuracies
    params['accuracy'] = ('accurate', 'dummy')
    with pytest.raises(ValueError):
      rfbp = rFBP(**params)
      assert params == rfbp.get_params()

    # test wrong protocol
    params['accuracy'] = ('accurate', 'exact')
    params['protocol'] = 'none'
    with pytest.raises(ValueError):
      rfbp = rFBP(**params)
      assert params == rfbp.get_params()

    # test wrong magnetization
    params['protocol'] = 'pseudo_reinforcement'

    class Magnetization:
      pass

    params['mag'] = Magnetization
    with pytest.raises(TypeError):
      rfbp = rFBP(**params)
      assert params == rfbp.get_params()


  def test_fit (self):

    params = { 'mag' : MagP64,
               'hidden' : 3,
               'max_iter' : 1,
               'seed' : 135,
               'damping' : .5,
               'accuracy' : ('accurate', 'exact'),
               'randfact' : .1,
               'epsil' : .5,
               'protocol' : 'pseudo_reinforcement',
               'size' : 101,
               'nth' : 2
              }

    rfbp = rFBP(**params)

    # train
    pattern = Pattern().random(shape=(20, 101))
    rfbp.fit(pattern)


  def test_predict (self):

    params = { 'mag' : MagP64,
               'hidden' : 3,
               'max_iter' : 1,
               'seed' : 135,
               'damping' : .5,
               'accuracy' : ('accurate', 'exact'),
               'randfact' : .1,
               'epsil' : .5,
               'protocol' : 'pseudo_reinforcement',
               'size' : 101,
               'nth' : 2
              }

    rfbp = rFBP(**params)

    Xtest = np.ones(shape=(10, params['size']), dtype=int)

    # check is fitted
    with pytest.raises(NotFittedError):
      pred_label = rfbp.predict(Xtest)
      assert pred_label is not None

    # train
    pattern = Pattern().random(shape=(20, 101))
    rfbp.fit(pattern)

    pred_label = rfbp.predict(pattern)

    predicted = rfbp.predict(pattern.data)

    assert np.allclose(pred_label, predicted)

  def test_load_dump (self):

    params = { 'mag' : MagP64,
               'hidden' : 3,
               'max_iter' : 1,
               'seed' : 135,
               'damping' : .5,
               'accuracy' : ('accurate', 'exact'),
               'randfact' : .1,
               'epsil' : .5,
               'protocol' : 'pseudo_reinforcement',
               'size' : 101,
               'nth' : 2
              }

    rfbp = rFBP(**params)

    with pytest.raises(NotFittedError):
      rfbp.save_weights('dummy.csv', delimiter=',', binary=False)

    pattern = Pattern().random(shape=(20, 101))
    rfbp.fit(pattern)

    W = rfbp.weights_

    rfbp.save_weights('dummy.csv', delimiter=',', binary=False)
    rfbp.load_weights('dummy.csv', delimiter=',', binary=False)

    os.remove('dummy.csv')

    assert np.allclose(W, rfbp.weights_)
    assert rfbp.hidden == params['hidden']

    W = rfbp.weights_
    rfbp.save_weights('dummy.bin', binary=True)
    rfbp.load_weights('dummy.bin', binary=True)

    os.remove('dummy.bin')

    assert np.allclose(W, rfbp.weights_)
    assert rfbp.hidden == params['hidden']
