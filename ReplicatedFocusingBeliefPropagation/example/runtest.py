#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import argparse
from ReplicatedFocusingBeliefPropagation import Pattern
from ReplicatedFocusingBeliefPropagation import ReplicatedFocusingBeliefPropagation as rFBP

try:

  from scorer import Scorer

  USE_SCORER = True

except ImportError:

  USE_SCORER = False


__author__  = ["Nico Curti", "Daniele Dall'Olio"]
__email__   = ['nico.curti2@unibo.it', 'daniele.dallolio@studio.unibo.it']


def parse_args():
  description = "rFBP test example"

  parser = argparse.ArgumentParser(description=description)
  parser.add_argument('--patterns',
                      dest='patterns',
                      required=True,
                      type=str,
                      action='store',
                      help='Pattern Filename' )
  parser.add_argument('--output',
                      dest='output',
                      required=False,
                      type=str,
                      action='store',
                      help='Output filename',
                      default='')
  parser.add_argument('--bin',
                      dest='bin',
                      required=False,
                      type=bool,
                      action='store',
                      help='File format: (0) Textfile(default), (1) Binary',
                      default=False,
                      choices=[0, 1])
  parser.add_argument('--weights',
                      dest='weights',
                      required=True,
                      type=str,
                      action='store',
                      help='Weights Matrix filename')
  parser.add_argument('--delimiter',
                      dest='delimiter',
                      required=False,
                      type=str,
                      action='store',
                      help='Delimiter for text file(default: \"\\t\")',
                      default='\t')

  args = parser.parse_args()

  return args


def test():

  args = parse_args()

  pattern = Pattern(args.patterns, args.bin, args.delimiter)

  rfbp = rFBP()
  rfbp.load_weights(args.weights)

  start_time = time.time()
  predicted_labels = rfbp.predict(pattern)
  elapsed_time = time.time() - start_time

  print ('{0}: Predicted in {1:.2f} seconds'.format(args.patterns, elapsed_time))

  if USE_SCORER:

    scorer = Scorer()
    scorer.evaluate(pattern.labels, predicted_labels)
    print(scorer)


if __name__ == '__main__':

  test()
