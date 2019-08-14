#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time

from ReplicatedFocusingBeliefPropagation import Mag
from ReplicatedFocusingBeliefPropagation import Pattern
from ReplicatedFocusingBeliefPropagation import Focusing_Protocol
from ReplicatedFocusingBeliefPropagation import ReplicatedFocusingBeliefPropagation as rFBP

try:

  from scorer import Scorer

  USE_SCORER = True

except ImportError:

  USE_SCORER = False

__package__ = "rFBP_test_example"
__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']



if __name__ == '__main__':

  pattern = Pattern(20,101)
  rfbp = rFBP(mag=Mag.magT,
              hidden=3,
              max_iter=1000,
              seed=135,
              damping=0.5,
              accuracy=['accurate','exact'],
              randfact=0.1,
              epsil=0.5,
              protocol='pseudo_reinforcement',
              size=101,
              nth=2)

  start_time = time.time()
  rfbp.fit(pattern)

  elapsed_time = time.time() - start_time
  print ('{0}: Training completed in {1:.2f} seconds'.format(pattern, elapsed_time))

  predicted_labels = rfbp.predict(pattern)
  print('Predictions:')

  if USE_SCORER:

    scorer = Scorer()
    scorer.evaluate(pattern.labels, predicted_labels)
    print(scorer)

  else:

    print(predicted_labels)

  print ('Done.')
