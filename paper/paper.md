---
title: 'rFBP: Replicated Focusing Belief Propagation algorithm'
tags:
  - belief-propagation
  - deep-neural-networks
  - sping-glass
  - statistical-mechanics
  - learning-algorithm
  - machine-learning-algorithms
  - python3
  - cpp17
authors:
 - name: Nico Curti [^a]
   orcid: 0000-0001-5802-1195
   affiliation: 1
 - name: Daniele Dall'Olio [^a]
   orcid: 0000-0003-0196-6870
   affiliation: 2
 - name: Daniel Remondini
   orcid: 0000-0003-3185-7456
   affiliation: 2
 - name: Gastone Castellani
   orcid: 0000-0003-4892-925X
   affiliation: 1
- name: Enrico Giampieri
   orcid: 0000-0003-2269-2338
   affiliation: 1
affiliations:
 - name: Department of Experimental, Diagnostic and Specialty Medicine of Bologna University
   index: 1
 - name: Department of Physics and Astronomy of Bologna University
   index: 2

date: 18 August 2020
bibliography: paper.bib
---

# Summary

The `rFBP` project implements a `scikit-learn` compatible machine-learning binary classifier leveraging fully connected neural networks with a learning algorithm (*Replicated Focusing Belief Propagation*, rFBP) that is fast converging and robust (less prone to brittle overfitting) for ill-posed datasets (very few samples compared to the number of features).
The current implementation works only with binary features such as one-hot encoding for categorical data.

This library has been already used to successfully predict *source attribution* starting from GWAS (*Genome Wide Association Studies*) data.
That study was trying to predict the animal origin for an infectious bacterial disease inside the H2020 European project COMPARE (Grant agreement ID: 643476).
A full description of the pipeline used in this study is available in the abstract and slides provided into the [publications](https://github.com/Nico-Curti/rFBP/blob/master/publications) folder of the project.

The learning problem under ill-posed conditions could be tackled through statistical mechanic models joined with the so-called Large Deviation Theory [@parisi2007mean; @Baldassi_2015; @Monasson_1995; @Zecchina_1995; @Baldassi_2016_local].
In general, the learning problem can be split into two sub-parts: the classification problem and the generalization one.
The first aims to completely store a pattern sample, i.e a prior known ensemble of input-output associations (*perfect learning*) [@Baldassi_2016; @Krauth1989StorageCO].
The second one corresponds to compute a discriminant function based on a set of features of the input which guarantees a unique association of a pattern.

From a statistical point-of-view many Neural Network models have been proposed and the most promising seems to be spin-glass models based.
Starting from a balanced distribution of the system, generally based on Boltzmann distribution, and under proper conditions, we can prove that the classification problem becomes a NP-complete computational problem [@Blum_1992].
A wide range of heuristic solutions to that type of problems were proposed [@Huang_2014; @Braunstein_2006; @Baldassi_11079].

In this project we show one of these algorithms developed by Zecchina et al. [@Baldassi_2016] and called *Replicated Focusing Belief Propagation* (`rFBP`).
The `rFBP` algorithm is a learning algorithm developed to justify the learning process of a binary neural network framework.
The model is based on a spin-glass distribution of neurons put on a fully connected neural network architecture.
In this way each neuron is identified by a spin and so only binary weights (-1 and 1) can be assumed by each entry.
The learning rule which controls the weight updates is given by the Belief Propagation method.

A first implementation of the algorithm was proposed in the original paper [@Baldassi_2016] jointly with an open-source Github repository.
The original version of the code was written in `Julia` language and despite it is a quite efficient implementation the `Julia` programming language stays on difficult and far from many users.
To broaden the scope and use of the method, a `C++` implementation was developed with a joint `Cython` wrap for `Python` users.
The `C++` language guarantees better computational performances against the `Julia` implementation and the `Python` version enhances its usability.
This implementation is optimized for parallel computing and is endowed with a custom `C++` library called [`Scorer`](https://github.com/Nico-Curti/scorer) for further details), which is able to compute a large number of statistical measurements based on a hierarchical graph scheme.
With this optimized implementation and its [`scikit-learn`](https://github.com/scikit-learn/scikit-learn) compatibility we try to encourage researchers to approach these alternative algorithms and to use them more frequently on real context.

As the `Julia` implementation also the `C++` one provides the entire `rFBP` framework in a single library callable via a command line interface.
The library widely uses template syntaxes to perform dynamic specialization of the methods between two magnetization versions of the algorithm.
The main object categories needed by the algorithm are wrapped in handy `C++` objects easy to use also from the `Python` interface.

Algorithm application on real data:

- *Classification of Genome Wide Association data by Belief Propagation Neural network*, [CCS Italy 2019](https://github.com/Nico-Curti/rFBP/blob/master/publications/conference/ccs19.pdf), Conference paper

- *Classification of Genome Wide Association data by Belief Propagation Neural network*, [CCS Italy 2019](https://github.com/Nico-Curti/rFBP/blob/master/publications/presentation/ccs19.pdf), Conference slides

# Acknowledgments

The authors acknowledge COMPARE n. 643476 EU Horizon 2020 (EU) Project.

[^a] : Both authors contributed equally to this work

# References
