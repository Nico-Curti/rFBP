#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

import pickle
import pandas as pd
# import numpy as np # for Python non-bayes test version
# import warnings # for Python non-bayes test version
# from scipy.special import erf # for Python non-bayes test version

from .FocusingProtocol import Focusing_Protocol
from .Patterns import Pattern

from .misc import _check_string
from sklearn.utils import check_array
from lib.ReplicatedFocusingBeliefPropagation.rFBP import Mag
from lib.ReplicatedFocusingBeliefPropagation.rFBP import _rfbp
from lib.ReplicatedFocusingBeliefPropagation.rFBP import _nonbayes_test
from sklearn.base import BaseEstimator

import multiprocessing

NTH = multiprocessing.cpu_count()


__package__ = "ReplicatedFocusingBeliefPropagation"
__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']



class ReplicatedFocusingBeliefPropagation(BaseEstimator):

  def __init__(self, mag=Mag.magP, hidden=3, max_iter=1000, seed=135, damping=0.5, accuracy=['accurate', 'exact'], randfact=1e-1, epsil=1e-1, protocol='pseudo_reinforcement', size=101, nth=NTH):
    '''
    ReplicatedFocusingBeliefPropagation classifier

    Parameters
    ----------
      mag : Enum Mag (default = MagP64)
        Switch magnetization type

      hidden : int (default = 3)
        Number of hidden layers

      max_iters : int (default = 1000)
        Number of iterations

      seed : int (default = 135)
        Random seed

      damping : float (default = 0.5)
        Damping parameter

      accuracy : pair of string (default : ('accurate', 'exact'))
        Accuracy of the messages computation at the hidden units level. Possible values are ('exact', 'accurate', 'approx', 'none')

      randfact : float (default = 0.1)
        Seed random generator of Cavity Messages

      epsil : float (default = 0.1)
        Threshold for convergence

      protocol : string (default = 'pseudo_reinforcement')
        Updating protocol. Possible values are ["scoping", "pseudo_reinforcement", "free_scoping", "standard_reinforcement"]

      size : int (default = 101)
        Number of updates

      nth : int (default = max_num_of_cores)
        Number of thread to use in the computation

    Members
    -------
      fit : Fit the model based on supervised input data

      predict : Predict labels on a new pattern given

      load_weights : Load a weight matrix from file in ascii or binary mode

      save_weights : Dump the fitted weights on file in ascii or binary mode
    '''

    if not isinstance(mag, Mag):
      raise TypeError('Magnetization must be an instance of Mag Enum')

    if len(accuracy) > 2:
      raise TypeError('Too many accuracies given. Max number is two')

    if accuracy[0] not in ('exact', 'accurate', 'approx', 'none') or accuracy[1] not in ('exact', 'accurate', 'approx', 'none'):
      raise ValueError('Wrong accuracy. Possible values are only ("exact", "accurate", "approx", "none"). Given: {}, {}'.format(accuracy[0], accuracy[1]))

    if protocol not in ['scoping', 'pseudo_reinforcement', 'free_scoping', 'standard_reinforcement']:
      raise ValueError('Incorrect Protocol found. Possible values are only ["scoping", "pseudo_reinforcement", "free_scoping", "standard_reinforcement"]')

    self._mag = mag
    self._hidden = hidden
    self._max_iter = max_iter
    self._seed = seed
    self._damping = damping
    self._accuracy = accuracy
    self._randfact = randfact
    self._epsil = epsil
    self._protocol = protocol
    self._size = size
    self._nth = nth
    self._weights = None

    self._fit = False


  def predict(self, X):
    '''
    Predict the new labels computed by ReplicatedFocusingBeliefPropagation model

    Parameters
    ----------
    X : array of shape [n_samples, n_features]
        The input samples.

    Returns
    -------
    y : array of shape [n_samples]
        The predicted target values.
    '''

    if not self._fit:
      raise ValueError('ReplicatedFocusingBeliefPropagation classifier is not fitted yet. Please use the fit method before predict')

    if not self._weights:
      raise ValueError("Weights must be computed before predict function. Use 'fit' function or 'load_weights' to read them from file")

    if isinstance(X, Pattern):
      # testset = X.data
      testset = X # use this with c++ version

    else:
      X = check_array(X)
      testset = Pattern(X, [0] * X.shape[0]) # use this with c++ version
      # testset = X
    return _nonbayes_test(self._weights, testset.pattern, self._hidden)

    # nrow, ncol = np.shape(testset)
    # predicted_labels = np.empty(nrow, dtype=int)
    #
    # with warnings.catch_warnings(): # catch runtime warning due to s2 == 0
    #   warnings.simplefilter("ignore")
    #
    #   for i, Xi in enumerate(testset):
    #     s = sum( [erf( sum([w * x for w, x in zip(Wj, Xi)])  / np.sqrt(2. * sum([ (1 - w * w) * x * x for w, x in zip(Wj, Xi)])))
    #             for Wj in self.weights] )
    #     predicted_labels[i] = np.sign(s)
    # return predicted_labels


  def fit(self, X, y=None):
    '''
    Fit the ReplicatedFocusingBeliefPropagation model meta-transformer

    Parameters
    ----------
      X : array-like of shape (n_samples, n_features)
          The training input samples.

      y : array-like, shape (n_samples,)
          The target values (integers that correspond to classes in
          classification, real numbers in regression).

    Returns
    -------
      self : object
        Returns self.
    '''

    self._fit = False

    if isinstance(X, Pattern):
      pattern = X

    else:
      pattern = Pattern(X, y)

    protocol = Focusing_Protocol(protocol=self._protocol, size=self._size)
    self._weights = _rfbp(mag=self._mag,
                          pattern=pattern.pattern,
                          protocol=protocol.fprotocol,
                          hidden=self._hidden,
                          max_iter=self._max_iter,
                          max_steps=protocol.num_of_replicas,
                          randfact=self._randfact,
                          damping=self._damping,
                          epsil=self._epsil,
                          accuracy=self._accuracy,
                          seed=self._seed,
                          nth=self._nth
                          )

    self._fit = True

    return self


  def load_weights(self, weightfile, delimiter='\t', binary=False):
    '''
    Load weights from file

    Parameters
    ----------
      weightfile : string
        Filename of weights

      delimiter : char
        Separator for ascii loading

      binary : bool
        Switch between binary and ascii loading style

    Returns
    -------
      self
    '''

    if binary:
      with open(weightfile, 'rb') as fp:
        self._weights = pickle.load(fp)

    else:
      self._weights = pd.read_csv(weightfile, sep=delimiter, header=None).values.tolist()

    self._hidden = len(self._weights)

    self._fit = True

    return self


  def save_weights(self, weightfile, delimiter='\t', binary=False):
    '''
    Load weights from file

    Parameters
    ----------
      weightfile : string
        Filename to dump the weights

      delimiter : char
        Separator for ascii dump

      binary : bool
        Switch between binary and ascii dumping style
    '''

    if binary:
      with open(weightfile, 'wb') as fp:
        pickle.dump(self._weights, fp)

    else:
      pd.DataFrame(self._weights).to_csv(weightfile, sep=delimiter, header=False, index=False)


  def __repr__(self):
    class_name = self.__class__.__name__
    return '<{} Class>'.format(class_name)
