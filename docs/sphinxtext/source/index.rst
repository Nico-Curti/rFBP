.. Replicated Focusing Belief Propagation algorithm documentation master file, created by
   sphinx-quickstart on Fri Oct  2 12:42:24 2020.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Replicated Focusing Belief Propagation's documentation!
==================================================================

The **Replicated Focusing Belief Propagation** package is inspired by the original BinaryCommitteeMachineFBP_ verion written in Julia.
In our implementation we optimize and extend the original library inclu multi-threading support and an easy-to-use interface to the main algorithm.
To further improve the usage of our code, we propose also a `Python` wrap of the library with a full compatibility with the `scikit-learn`_ and `scikit-optimize`_ packages.

Overview
========

The learning problem could be faced through statistical mechanic models joined with the so-called Large Deviation Theory.
In general, the learning problem can be split into two sub-parts: the classification problem and the generalization one.
The first aims to completely store a pattern sample, i.e a prior known ensemble of input-output associations (*perfect learning*).
The second one corresponds to compute a discriminant function based on a set of features of the input which guarantees a unique association of a pattern.

From a statistical point-of-view many Neural Network models have been proposed and the most promising seems to be spin-glass models based.
Starting from a balanced distribution of the system, generally based on Boltzmann distribution, and under proper conditions, we can prove that the classification problem becomes a NP-complete computational problem.
A wide range of heuristic solutions to that type of problems were proposed.

In this project we show one of these algorithms developed by `Zecchina et al`_ and called *Replicated Focusing Belief Propagation* (`rFBP`).
The `rFBP` algorithm is a learning algorithm developed to justify the learning process of a binary neural network framework.
The model is based on a spin-glass distribution of neurons put on a fully connected neural network architecture.
In this way each neuron is identified by a spin and so only binary weights (-1 and 1) can be assumed by each entry.
The learning rule which controls the weight updates is given by the Belief Propagation method.

A first implementation of the algorithm was proposed in the original paper (`Zecchina et al`_) jointly with an open-source Github repository.
The original version of the code was written in `Julia` language and despite it is a quite efficient implementation the `Julia` programming language stays on difficult and far from many users.
To broaden the scope and use of the method, a `C++` implementation was developed with a joint `Cython` wrap for `Python` users.
The `C++` language guarantees better computational performances against the `Julia` implementation and the `Python` version enhances its usability.
This implementation is optimized for parallel computing and is endowed with a custom `C++` library called scorer_, which is able to compute a large number of statistical measurements based on a hierarchical graph scheme.
With this optimized implementation and its `scikit-learn`_ compatibility we try to encourage researchers to approach these alternative algorithms and to use them more frequently on real context.

As the `Julia` implementation also the `C++` one provides the entire `rFBP` framework in a single library callable via a command line interface.
The library widely uses template syntaxes to perform dynamic specialization of the methods between two magnetization versions of the algorithm.
The main object categories needed by the algorithm are wrapped in handy `C++` objects easy to use also from the `Python` interface.


Usage example
-------------

The `rfbp` object is totally equivalent to a `scikit-learn` classifier and thus it provides the member functions `fit` (to train your model) and `predict` (to test a trained model on new samples).

.. code-block:: python

   import numpy as np
   from sklearn.model_selection import train_test_split
   from ReplicatedFocusingBeliefPropagation import MagT64
   from ReplicatedFocusingBeliefPropagation import Pattern
   from ReplicatedFocusingBeliefPropagation import ReplicatedFocusingBeliefPropagation as rFBP

   N, M = (20, 101) # M must be odd
   X = np.random.choice([-1, 1], p=[.5, .5], size=(N, M))
   y = np.random.choice([-1, 1], p=[.5, .5], size=(N, ))

   X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.33, random_state=42)

   rfbp = rFBP(mag=MagT64,
               hidden=3,
               max_iter=1000,
               seed=135,
               damping=0.5,
               accuracy=('accurate','exact'),
               randfact=0.1,
               epsil=0.5,
               protocol='pseudo_reinforcement',
               size=101,
               nth=1)

   rfbp.fit(X_train, y=y_train)
   y_pred = rfbp.predict(X_test)

The same code could be easily translated also in a pure C++ application as

.. code-block:: c++

   #include <rfbp.hpp>

   int main ()
   {
     const int N = 20;
     const int M = 101; // M must be odd
     const int K = 3;

     FocusingProtocol fp("pseudo_reinforcement", M);
     Patterns patterns(N, M);

     long int ** bin_weights = focusingBP < MagP64 >(K,          // hidden,
                                                     patterns,   // patterns,
                                                     1000,       // max_iters,
                                                     101,        // max_steps,
                                                     42,         // seed,
                                                     0.5,        // damping,
                                                     "accurate", // accuracy1,
                                                     "exact",    // accuracy2,
                                                     0.1,        // randfact,
                                                     fp,         // fp,
                                                     0.1,        // epsil,
                                                     1,          // nth,
                                                     "",         // outfile,
                                                     "",         // outmess,
                                                     "",         // inmess,
                                                     false       // binmess
                                                     );

     // It is clearly an overfitting! But it works as example
     long int ** y_pred = nonbayes_test(bin_weights, patterns, K);

     return 0;
   }


.. _BinaryCommitteeMachineFBP: https://github.com/carlobaldassi/BinaryCommitteeMachineFBP.jl
.. _`scikit-learn`: https://github.com/scikit-learn/scikit-learn
.. _`scikit-optimize`: https://github.com/scikit-optimize/scikit-optimize
.. _`Zecchina et al`: https://www.pnas.org/content/113/48/E7655
.. _scorer: https://github.com/Nico-Curti/scorer


.. toctree::
   :maxdepth: 2
   :caption: Contents:

   theory
   installation
   cppAPI/modules
   pyAPI/modules
   references

