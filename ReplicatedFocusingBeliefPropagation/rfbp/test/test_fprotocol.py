#!/usr/bin/env python
# -*- coding: utf-8 -*-

from __future__ import print_function
from __future__ import division

from ReplicatedFocusingBeliefPropagation import Focusing_Protocol

__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


class TestProtocol:
  '''
  Test Focusing_Protocol object and its wrap in Cython
  '''

  MAX_STEPS = 101

  def test_scoping (self):
    '''
    Tests:
      - array size
      - first and last values
      - incremental range
      - uniform arrays
      - infinity of beta
    '''

    fprotocol = Focusing_Protocol('scoping', self.MAX_STEPS)
    print(fprotocol)

    # check size
    assert fprotocol.num_of_replicas == self.MAX_STEPS;

    # check starting and last values
    assert fprotocol.gamma[0] == 0.
    assert fprotocol.gamma[self.MAX_STEPS - 1] == float('inf')
    assert fprotocol.n_rep[0] == 21. # 21 is a magic number ??

    # check increment range
    assert sorted(fprotocol.gamma) == fprotocol.gamma

    # check all values equal
    assert len(set(fprotocol.n_rep)) == 1
    assert len(set(fprotocol.beta)) == 1

    # check inf beta
    assert fprotocol.beta[0] == float('inf')


  def test_pseudo_reinforcement (self):
    '''
    Tests:
      - array size
      - first and last values
      - incremental range
      - uniform arrays
      - infinity of beta
    '''

    fprotocol = Focusing_Protocol('pseudo_reinforcement', self.MAX_STEPS)
    print(fprotocol)

    # check size
    assert fprotocol.num_of_replicas == self.MAX_STEPS;

    # check starting and last values
    assert fprotocol.gamma[0] == 0.
    assert fprotocol.gamma[self.MAX_STEPS - 1] == float('inf')
    assert fprotocol.n_rep[0] == 1.

    # check increment range
    assert sorted(fprotocol.gamma) == fprotocol.gamma
    assert sorted(fprotocol.n_rep) == fprotocol.n_rep

    # check all values equal
    assert len(set(fprotocol.beta)) == 1

    # check inf beta
    assert fprotocol.beta[0] == float('inf')


  def test_free_scoping (self):
    '''
    Tests:
      - array size
      - first and last values
      - incremental range
      - uniform arrays
      - infinity of beta
    '''

    fprotocol = Focusing_Protocol('free_scoping', self.MAX_STEPS)
    print(fprotocol)

    # check size
    assert fprotocol.num_of_replicas == self.MAX_STEPS;

    # check starting and last values
    assert fprotocol.gamma[0] == 1.
    assert fprotocol.gamma[self.MAX_STEPS - 1] == float('inf')
    assert fprotocol.n_rep[0] == 2.
    assert fprotocol.n_rep[self.MAX_STEPS - 1] == float('inf')

    # check increment range
    assert sorted(fprotocol.gamma) == fprotocol.gamma
    assert sorted(fprotocol.n_rep) == fprotocol.n_rep

    # check all values equal
    assert len(set(fprotocol.beta)) == 1

    # check inf beta
    assert fprotocol.beta[0] == float('inf')


  def test_standard_reinforcement (self):
    '''
    Tests:
      - array size
      - first and last values
      - incremental range
      - uniform arrays
      - infinity of beta
    '''

    fprotocol = Focusing_Protocol('standard_reinforcement', self.MAX_STEPS)
    print(fprotocol)

    # check size
    assert fprotocol.num_of_replicas == self.MAX_STEPS;

    # check starting and last values
    assert fprotocol.n_rep[0] == 1.
    assert fprotocol.gamma[0] == float('inf')
    assert fprotocol.beta[0] == float('inf')

    # check increment range
    assert sorted(fprotocol.n_rep) == fprotocol.n_rep

    # check all values equal
    assert len(set(fprotocol.gamma)) == 1
    assert len(set(fprotocol.beta)) == 1



  def test_wrong_protocol (self):

    try:

      fprotocol = Focusing_Protocol('new_protocol', self.MAX_STEPS)
      print(fprotocol)
      assert False

    except ValueError:

      pass

  def test_wrong_size (self):

    try:

      fprotocol = Focusing_Protocol('scoping', 0)
      print(fprotocol)
      assert False

    except ValueError:

      assert True

    try:

      fprotocol = Focusing_Protocol('scoping', 1)
      print(fprotocol)
      assert False

    except ValueError:

      assert True


if __name__ == '__main__':

  test = TestProtocol()
  test.test_scoping()
  test.test_pseudo_reinforcement()
  test.test_free_scoping()
  test.test_standard_reinforcement()
  test.test_wrong_protocol()
  test.test_wrong_size()
