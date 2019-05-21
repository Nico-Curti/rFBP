#!/usr/bin/env python

import pickle
import pandas as pd
# import numpy as np # for Python non-bayes test version
# import warnings # for Python non-bayes test version
# from scipy.special import erf # for Python non-bayes test version

from ReplicatedFocusingBeliefPropagation.source.FocusingProtocol import Focusing_Protocol
from ReplicatedFocusingBeliefPropagation.source.Patterns import Pattern
from ReplicatedFocusingBeliefPropagation import Mag

from sklearn.utils import check_array
from ReplicatedFocusingBeliefPropagation.source.misc import _check_string
from lib.ReplicatedFocusingBeliefPropagation.rFBP import _rfbp, _nonbayes_test
from sklearn.base import BaseEstimator

__package__ = "ReplicatedFocusingBeliefPropagation"
__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']

import multiprocessing
NTH = multiprocessing.cpu_count()

class ReplicatedFocusingBeliefPropagation(BaseEstimator):

  def __init__(self,
                     mag = Mag.magP,
                     hidden = 3,
                     max_iter = 1000,
                     seed = 135,
                     damping = 0.5,
                     accuracy = ['accurate', 'exact'],
                     randfact = 1e-1,
                     epsil = 1e-1,
                     protocol = 'pseudo_reinforcement',
                     size = 101,
                     nth = NTH):
    if not isinstance(mag, Mag):
      raise TypeError('Magnetization must be an instance of Mag Enum')

    self.mag = mag
    self.hidden = hidden
    self.max_iter = max_iter
    self.seed = seed
    self.damping = damping
    self.accuracy = accuracy
    self.randfact = randfact
    self.epsil = epsil
    self.protocol = protocol
    self.size = size
    self.nth = nth
    self.weights = None


  def predict(self, X):
    if not self.weights:
      raise ValueError("Weights must be computed before predict function. Use 'fit' function or 'load_weights' to read them from file")

    if isinstance(X, Pattern):
      # testset = X.data
      testset = X # use this with c++ version
    else:
      X = check_array(X)
      testset = Pattern(X, [0] * X.shape[0]) # use this with c++ version
      # testset = X
    return _nonbayes_test(self.weights, testset.pattern, self.hidden)

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


  def fit(self, X, y = None):
    if isinstance(X, Pattern):
      pattern = X
    else:
      pattern = Pattern(X, y)

    protocol = Focusing_Protocol(protocol=self.protocol, size=self.size)
    self.weights = _rfbp(mag = self.mag,
                         pattern = pattern.pattern,
                         protocol = protocol.fprotocol,
                         hidden = self.hidden,
                         max_iter = self.max_iter,
                         max_steps = protocol.nrep,
                         randfact = self.randfact,
                         damping = self.damping,
                         epsil = self.epsil,
                         accuracy = self.accuracy,
                         seed = self.seed,
                         nth = self.nth
                         )


  def load_weights(self, weightfile, delimiter='\t', binary=False):
    if binary:
      with open(weightfile, 'rb') as fp:
        self.weights = pickle.load(fp)
    else:
      self.weights = pd.read_csv(weightfile, sep=delimiter, header=None).values.tolist()

    self.hidden = len(self.weights)


  def save_weights(self, weightfile, delimiter='\t', binary=False):
    if binary:
      with open(weightfile, 'wb') as fp:
        pickle.dump(self.weights, fp)
    else:
      pd.DataFrame(self.weights).to_csv(weightfile, sep=delimiter, header=False, index=False)


  def __repr__(self):
    class_name = self.__class__.__name__
    return '<%s Class>'%(class_name)
