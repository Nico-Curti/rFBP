| **Authors**  | **Project** |  **Build Status** | **Code Quality** | **Coverage** |
|:------------:|:-----------:|:-----------------:|:----------------:|:------------:|
| [**N. Curti**](https://github.com/Nico-Curti) <br/> [**D. Dall'Olio**](https://github.com/DanieleDallOlio) <br/> [**E. Giampieri**](https://github.com/EnricoGiampieri)  |  **rFBP**  | **Linux/MacOS** : [![Travis](https://travis-ci.com/Nico-Curti/rFBP.svg?token=7QqsqaQiuDHSyGDT3xek&branch=master)](https://travis-ci.com/Nico-Curti/rFBP) <br/> **Windows** : [![appveyor](https://ci.appveyor.com/api/projects/status/obuq56lhyd90pmup?svg=true)](https://ci.appveyor.com/project/Nico-Curti/rfbp) | **Codacy** : [![Codacy](https://api.codacy.com/project/badge/Grade/a6fdac990b6f4141a5bd9e8171ddaf53)](https://www.codacy.com/manual/Nico-Curti/rFBP?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Nico-Curti/rFBP&amp;utm_campaign=Badge_Grade) <br/> **Codebeat** : [![Codebeat](https://codebeat.co/badges/cc761a7c-79fa-4a66-984f-bef6fd145d34)](https://codebeat.co/projects/github-com-nico-curti-rfbp-master) | [![codecov](https://codecov.io/gh/Nico-Curti/rFBP/branch/master/graph/badge.svg)](https://codecov.io/gh/Nico-Curti/rFBP) |

[![rFBP C++ CI](https://github.com/Nico-Curti/rFBP/workflows/rFBP%20C++%20CI/badge.svg)](https://github.com/Nico-Curti/rFBP/actions?query=workflow%3A%22rFBP+C%2B%2B+CI%22)
[![rFBP Python CI](https://github.com/Nico-Curti/rFBP/workflows/rFBP%20Python%20CI/badge.svg)](https://github.com/Nico-Curti/rFBP/actions?query=workflow%3A%22rFBP+Python+CI%22)

[![docs](https://img.shields.io/badge/doc-latest-blue.svg?style=plastic)](https://nico-curti.github.io/rFBP/)
[![GitHub pull-requests](https://img.shields.io/github/issues-pr/Nico-Curti/rFBP.svg?style=plastic)](https://github.com/Nico-Curti/rFBP/pulls)
[![GitHub issues](https://img.shields.io/github/issues/Nico-Curti/rFBP.svg?style=plastic)](https://github.com/Nico-Curti/rFBP/issues)

[![GitHub stars](https://img.shields.io/github/stars/Nico-Curti/rFBP.svg?label=Stars&style=social)](https://github.com/Nico-Curti/rFBP/stargazers)
[![GitHub watchers](https://img.shields.io/github/watchers/Nico-Curti/rFBP.svg?label=Watch&style=social)](https://github.com/Nico-Curti/rFBP/watchers)

<a href="https://github.com/UniboDIFABiophysics">
<div class="image">
<img src="https://cdn.rawgit.com/physycom/templates/697b327d/logo_unibo.png" width="90" height="90">
</div>
</a>

# Replicated Focusing Belief Propagation algorithm

We propose a `C++` version of the [**Replicated Focusing Belief Propagation**](https://github.com/carlobaldassi/BinaryCommitteeMachineFBP.jl) Julia package.
Our implementation optimizes and extends the original library including multi-threading support and an easy-to-use interface to the main algorithm.
To further improve the usage of our code, we propose also a `Python` wrap of the library with a full compatibility with the [`scikit-learn`](https://github.com/scikit-learn/scikit-learn) package.

* [Overview](#overview)
* [Theory](#theory)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
* [Efficiency](#efficiency)
* [Usage](#usage)
* [Testing](#testing)
* [Table of contents](#table-of-contents)
* [Contribution](#contribution)
* [References](#references)
* [Authors](#authors)
* [License](#license)
* [Acknowledgments](#acknowledgments)
* [Citation](#citation)

## Overview

The learning problem could be faced through statistical mechanic models joined with the so-called Large Deviation Theory.
In general, the learning problem can be split in two sub-parts: the classification problem and the generalization one.
The first aims to completely store a pattern sample, i.e a prior known ensemble of input-output associations (*perfect learning*).
The second one corresponds to compute a discriminant function based on a set of features of the input which guarantees a unique association of a pattern.

From a statistical point-of-view many Neural Network models have been proposed and the most promising seems to be spin-glass models based.
Starting from a balanced distribution of the system, generally based on Boltzmann distribution, and under proper conditions, we can prove that the classification problem becomes a NP-complete computational problem.
A wide range of heuristic solutions to that type of problems were proposed.

In this project we show one of these algorithms developed by Zecchina et al. [[BaldassiE7655](https://www.pnas.org/content/113/48/E7655)] and called *Replicated Focusing Belief Propagation* (`rFBP`).
The `rFBP` algorithm is a learning algorithm developed to justify the learning process of a binary neural network framework.
The model is based on a spin-glass distribution of neurons put on a fully connected neural network architecture.
In this way each neuron is identified by a spin and so only binary weights (-1 and 1) can be assumed by each entry.
The learning rule which controls the weight updates is given by the Belief Propagation method.

A first implementation of the algorithm was proposed in the original paper [[BaldassiE7655](https://www.pnas.org/content/113/48/E7655)] jointly with an open-source Github repository.
The original version of the code was written in `Julia` language and despite it is a quite efficient implementation the `Julia` programming language stays on difficult and far from many users.
To broaden the scope and use of the method, a `C++` implementation was developed with a joint `Cython` wrap for `Python` users.
The `C++` language guarantees better computational performances against the `Julia` implementation and the `Python` version enhance its usability.
This implementation is optimized for parallel computing and is endowed with a custom `C++` library called [`Scorer`](https://github.com/Nico-Curti/scorer)), which is able to compute a large number of statistical measurements based on a hierarchical graph scheme.
With this optimized implementation and its [`scikit-learn`](https://github.com/scikit-learn/scikit-learn) compatibility we try to encourage researchers to approach these alternative algorithms and to use them more frequently on real context.

As the `Julia` implementation also the `C++` one provides the entire `rFBP` framework in a single library callable via a command line interface.
The library widely uses template syntaxes to perform dynamic specialization of the methods between two magnetization versions of the algorithm.
The main object categories needed by the algorithm are wrapped in handy `C++` objects easy to use also from the `Python` interface.

## Theory

The `rFBP` algorithm derives from an out-of-equilibrium (non-Boltzmann) model of the learning process of binary neural networks [[DallAsta101103](https://journals.aps.org/pre/abstract/10.1103/PhysRevE.77.031118)].
This model mimics a spin glass system whose realizations are equally likely to occur when sharing the same so-called entropy (not the same energy, i.e. out-of-equilibrium).
This entropy basically counts the number of solutions (zero-energy realizations) around a realization below a fixed-distance.

Within this out-of-equilibrium framework, the objective is to maximize the entropy instead of minimizing the energy.
From a machine learning standpoint, we aim at those weights sets that perfectly solve the learning process (zero-errors) and that are mathematically closed to each other.
To this end, the Belief Propagation method [[MézardMontanari](https://web.stanford.edu/~montanar/RESEARCH/book.html)] can be adopted as the underlying learning rule, although it must properly adjusted to take into account the out-of-equilibrium nature of the model.

The model defines two parameters: y and <img src="https://render.githubusercontent.com/render/math?math={\gamma}">.
The former is a temperature-alike related variable, similar to the one usually exploited by Gradient Descend approaches, but it can be also interpreted as the number of interacting replicas of the system.
The latter sets the largest distance for surrounding solutions that are considered by entropy.
The Belief Propagation method needs to be to adjusted by adding incoming extra messages for all weights, in order to involve the interacting replicas of the system.
This extra term is represented by:

<img src="https://render.githubusercontent.com/render/math?math={\hat{m}^{t + 1}_{\star \to \w_i} = tanh \big[ (y-1) artanh ( m^t_{\w_i \to \star} tanh \gamma ) \big] tanh \gamma}">,

where <img src="https://render.githubusercontent.com/render/math?math={\w_i}"> and <img src="https://render.githubusercontent.com/render/math?math={\star}"> stand respectively for the i-th weight and a representation of all replicas.

The `rFBP` is therefore an adjusted Belief Propagation algorithm, whose general procedure can be summarized as follows:
- select protocols for y and <img src="https://render.githubusercontent.com/render/math?math={\gamma}">;
- set first values of y and <img src="https://render.githubusercontent.com/render/math?math={\gamma}"> and run the adjusted-BP method until convergence (<img src="https://render.githubusercontent.com/render/math?math={ < \epsilon}">) or up to a limited-number of iterations;
- step to the next pair values of y and <img src="https://render.githubusercontent.com/render/math?math={\gamma}"> with respect to the chosen protocols and re-run the adjusted-BP method;
- keep it going until a solution is reached or protocols end.

The `rFBP` algorithm focuses step by step the replicated system to fall into weights sets extremely closed to many perfect solutions, which ables them to well generalize out of the training set [[Baldassi101103](https://journals.aps.org/prl/pdf/10.1103/PhysRevLett.115.128101)].

## Prerequisites

C++ supported compilers:

![gcc version](https://img.shields.io/badge/gcc-4.8.5%20|%204.9.*%20|%205.*%20|%206.*%20|%207.*%20|%208.*%20|%209.*-yellow.svg)

![clang version](https://img.shields.io/badge/clang-3.6%20|3.9%20|5.*%20|%206.*%20|%207.*%20|-red.svg)

![msvc version](https://img.shields.io/badge/msvc-vs2017%20x86%20|%20vs2017%20x64-blue.svg)

The `rFBP` project is written in `C++` using a large amount of c++17 features.
To enlarge the usability of our package we provide also a retro-compatibility of all the c++17 modules reaching an usability (tested) of our code from gcc 4.8.5+.
The package installation can be performed via [`CMake`](https://github.com/Nico-Curti/rFBP/blob/master/CMakeLists.txt) or [`Makefile`](https://github.com/Nico-Curti/rFBP/blob/master/Makefile).
If you are using the `CMake` (recommended) installer the maximum version of C++ standard is automatic detected.

You can use also the `rFBP` package in `Python` using the `Cython` wrap provided inside this project.
The only requirements are the following:

* numpy >= 1.15
* cython >= 0.29
* scipy >= 1.2.1
* scikit-learn >= 0.20.3
* requests >= 2.22.0

The `Python` wrap guarantees also a good integration with the other common Machine Learning tools provided by `scikit-learn` `Python` package; in this way we can use the `rFBP` algorithm as equivalent alternative also in other pipelines.
Like other Machine Learning algorithm also the `rFBP` one depends on many parameters, i.e its hyper-parameters, which has to be tuned according to the given problem.
The `Python` wrap of the library was written according to `scikit-optimize` `Python` package to allow an easy hyper-parameters optimization using the already implemented classical methods.

## Installation

Follow the instruction about your needs.

A complete list of instructions "for beginners" is also provided for both [cpp](https://github.com/Nico-Curti/rFBP/blob/master/docs/cpp_install.md) and [python](https://github.com/Nico-Curti/rFBP/blob/master/docs/python_install.md) versions.

### CMake C++ installation

We recommend the use of `CMake` for the installation since it is the most automated way to reach your needs.
First of all make sure you have a sufficient version of `CMake` installed (3.9 minimum version required).
If you are working on a machine without root privileges and you need to upgrade your `CMake` version a valid solution to overcome your problems is provided [here](https://github.com/Nico-Curti/Shut).

With a valid `CMake` version installed first of all clone the project as:

```bash
git clone https://github.com/Nico-Curti/rFBP
cd rFBP
```

The you can build the `rFBP` package with

```bash
mkdir -p build
cd build && cmake .. && cmake --build . --target install
```

or more easily

```bash
./build.sh
```

if you are working on a Windows machine the right script to call is the [`build.ps1`](https://Nico-Curti/rFBP/blob/master/build.ps1).

**NOTE 1:** if you want enable the OpenMP support (*4.5 version is required*) compile the library with `-DOMP=ON`.

**NOTE 2:** if you use MagT configuration, please download the `atanherf coefficients` file before running any executable. You can find a downloader script inside the [scripts](https://github.com/Nico-Curti/rFBP/tree/master/scripts) folder. Enter in that folder and just run `python dowload_atanherf.py`.

### Make C++ installation

The `Make` installation requires more attention!
First of all the `Make` installation assumes that you compiler is able to support the c++17 standard: if it is not your case you have to change the `STD` variable into the `Makefile` script.

Then if you call just:

```bash
make
```

you can view the complete list of available examples.
With

```bash
make main
```

you can compile the main example and the `C++` library.

### Python installation

Python version supported : ![Python version](https://img.shields.io/badge/python-3.5|3.6|3.7|3.8-blue.svg)

The `Python` installation can be performed with or without the `C++` installation.
The `Python` installation is always executed using [`setup.py`](https://github.com/Nico-Curti/blob/master/setup.py) script.

If you have already build the `rFBP` `C++` library the installation is performed faster and the `Cython` wrap directly links to the last library installed.
Otherwise the full list of dependencies is build.

In both cases the installation steps are

```bash
python -m pip install -r ./requirements.txt
```

to install the prerequisites and then

```bash
python setup.py install
```

or for installing in development mode:

```bash
python setup.py develop --user
```

## Efficiency

![Comparison of time performances between the original `Julia` implementation and our `Cython` one of the `rFBP` algorithm varying the input dimension sizes (number of samples, `M`, and number of features, `N`). For each input configuration 100 runs of both algorithm were performed and the results were normalized by the `Julia` implementation. In these cases we fixed the magnetization to **MagP64**.](./img/rfbp_magp_timing.svg)

![Comparison of time performances between the original `Julia` implementation and our `Cython` one of the `rFBP` algorithm varying the input dimension sizes (number of samples, `M`, and number of features, `N`). For each input configuration 100 runs of both algorithm were performed and the results were normalized by the `Julia` implementation. In these cases we fixed the magnetization to **MagT64**.](./img/rfbp_magt_timing.svg)

We test the computational efficiency of our implementation against the original `Julia` one.
The tests were performed comparing our `Cython` version of the code (and thus with a slight overhead given by the `Python` interpreter) and the `Julia` implementation.
Varying the dimension sizes (number of samples, `M`, and number of features, `N`) we performed 100 runs of both the algorithms.
We divided our simulation according to the two possible types of magnetizations (`MagP64` and `MagT64`, as described in the original implementation available [here](https://github.com/carlobaldassi/BinaryCommitteeMachineFBP.jl)) and the obtained results are showed in Fig. [[1](./img/rgbp_magp_timing.svg), [2](./img/rgbp_magt_timing.svg)], respectively.

As can be seen by the two simulations our implementation scales very well with the number of samples and it is quite stable in relation to the number of features.
However, we can not guarantee a perfect parallel execution of our version: also with multi-threading support the scalability of our implementation does not follow a linear trend with the number of available cores.
In our simulation, in fact, we used 32 cores against the single thread execution of the `Julia` implementation but we gained only a 4x and 2x of speedup for `MagT64` and `MagP64`, respectively.
The network training is a sequential process by definition and thus it is hard to obtain a relevant speedup using a parallel implementation.
In this case it is probably jointed to a not perfect parallelization strategy chosen which bring to a not efficient scalability of our algorithm version.
However, the improvements performed to the code allow us to use this algorithm with bigger dataset sizes.

## Usage

You can use the `rFBP` library into pure-Python modules or inside your C++ application.

### C++ Version

The easiest usage of `rFBP` library is given by the two examples provided in the [example](https://github.com/Nico-Curti/rFBP/blob/master/example) folder.
These two scripts include an easy-to-use command line support for both training and test procedure.

To train the model you can just use

```
./bin/train_main
Usage: ./train_main [-threads <std::remove_reference<int>> ] -f <std :: string> [-output <std :: string> ] [-bin <std::remove_reference<bool>> ] [-delimiter <std :: string> ] [-hidden <std::remove_reference<int>> ] [-iteration <std::remove_reference<int>> ] [-seed <std::remove_reference<int>> ] [-randfact <std::remove_reference<double>> ] [-damping <std::remove_reference<double>> ] [-accuracy <std :: string> ] [-protocol <std :: string> ] [-epsilon <std::remove_reference<double>> ] [-steps <std::remove_reference<int>> ] [-mag <std::remove_reference<int>> ] [-inmess <std :: string> ] [-outmess <std :: string> ] [-delmess <std :: string> ] [-binmess <std::remove_reference<bool>> ]

Training BeliefPropagation ${VERSION}

optional arguments:
        -t,   --threads                 Max number of threads exploitable
        -f,   --file                    Pattern Filename (with extension)
        -o,   --output                  Output Filename (with extension)
        -b,   --bin                     File format: (0) Textfile(default), (1) Binary
        -dl,  --delimiter               Delimiter for text files(default: "\t")
        -k,   --hidden                  Number of Hidden Layers(default:3)
        -i,   --iteration               Max Number of Iterations(default: 1000)
        -r,   --seed                    Seed random generator(default: 135)
        -g,   --randfact                Seed random generator of Cavity Messages(default: 0.1)
        -d,   --damping                 Damping parameter(default: 0.5)
        -a,   --accuracy                Accuracy of the messages computation at the hidden units level (choose between 'exact'(default), 'accurate', 'approx', 'none')
        -p,   --protocol                Specify protocol : scooping, pseudo_reinforcement (default), free_scoping, standard_reinforcement
        -e,   --epsilon                 Threshold for convergence(default: 0.1)
        -s,   --steps                   Max Number of Steps for chosen protocol(default: 101)
        -m,   --mag                     Specify Magnetization: (0) MagnetizationP (MagP64), (1) MagnetizationT (MagT64)
        -im,  --inmess                  Input Messages file
        -om,  --outmess                 Output Messages file
        -dm,  --delmess                 Delimiter for Messages files(default: "\t")
        -bm,  --binmess                 Messages files format: (0) Textfile(default), (1) Binary
```

and after training you can test your model using

```
./bin/test_main
Usage: ./test_main [-threads <std::remove_reference<int>> ] -f <std :: string> [-bin <std::remove_reference<bool>> ] -w <std :: string> [-delimiter <std :: string> ] [-output <std :: string> ]

Test BeliefPropagation ${VERSION}

optional arguments:
        -t,   --threads                 Max number of threads exploitable
        -f,   --file                    Pattern Filename (with extension)
        -b,   --bin                     File format: (0) Textfile(default), (1) Binary
        -w,   --weights                 Weights Matrix Filename (with extension)
        -dl,  --delimiter               Delimiter for text files(default: "\t")
        -o,   --output                  Output Filename (no extension)
```

If you are interested in using `rFBP` inside your code you can simply import the [`rfbp.hpp`](https://github.com/Nico-Curti/rFBP/blob/master/hpp/rfbp.hpp) and create a `ReplicatedFocusingBeliefPropagation` object.

Then all the work is performed by the `focusingBP` (template) function.
You can use it with `MagP64` type or `MagT64` for more accurate (but slower) results.

The input pattern must be wrapped into a `Pattern` object provided by the library.

```c++
#include <rfbp.hpp>

int main ()
{
  FocusingProtocol fp("pseudo_reinforcement", 101);
  Patterns patterns("patternsfile.csv", false, ",");

  long int ** bin_weights = focusingBP < MagP64 >(3,          // K,
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

  return 0;
}
```

Then you can use the `nonbayes_test` function to predict your test set.

### Python Version

The `rfbp` object is totally equivalent to a `scikit-learn` classifier and thus it provides the member functions `fit` (to train your model) and `predict` (to test a trained model on new samples).

First of all you need to import the `rFBP` modules.

```python
from ReplicatedFocusingBeliefPropagation import MagT64
from ReplicatedFocusingBeliefPropagation import Pattern
from ReplicatedFocusingBeliefPropagation import ReplicatedFocusingBeliefPropagation as rFBP
```

If your want to run your script with multiple cores you can simply import also

```python
from ReplicatedFocusingBeliefPropagation import NTH
```

which is set to the maximum number of core in your computer.

You can start to try the package functionality using a random pattern

```python
N, M = (20, 101) # M must be odd
data = np.random.choice([-1, 1], p=[.5, .5], size=(20, 101))
label = np.random.choice([-1, 1], p=[.5, .5], size=(20, ))
```

The input data must be composed by binary variables codified as `[-1, 1]`, since the model works only with spin-like variables.
The next step is the creation of the `Replicated Focusing Belief Propagation` model.

```python
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
            nth=NTH)
```

Now you can fit your model and predict:

```python
rfbp.fit(data, label)
predicted_labels = rfbp.predict(data)
```

which is clearly an overfitting! But it works as example :blush:

The internal implementation of the algorithm works with a custom data type called `Pattern` (ref. [here](https://github.com/Nico-Curti/rFBP/blob/master/ReplicatedFocusingBeliefPropagation/rfbp/Patterns.py)).
You can explicitly use a `Pattern` object or convert your data to it

```python
data = np.random.choice(a=(-1, 1), p=(.5, .5), size=(n_sample, n_feature))
labels = np.random.choice(a=(-1, 1), p=(.5, .5), size=(n_sample,))

pt = Pattern(X=data, y=labels)
# dimensions
assert pt.shape == (n_sample, n_feature)
# data
np.testing.assert_allclose(pt.data, data)
# labels
np.testing.assert_allclose(pt.labels, labels)
```

We suggest the usage of this data type if you have to load your data from file.
We check the consistency of the input variables into the `C++` code (**only** in DEBUG mode) and into the `Python` wrap.

In the [example](https://github.com/Nico-Curti/rFBP/blob/master/ReplicatedFocusingBeliefPropagation/example/) folder you can find a training/test example using a pattern imported from file (a more realistic example).
Both the `fit` and `predict` functions works using either a `numpy` array and a `Pattern` object.

## Testing

`rFBP` uses CMake to build a full list of tests. You can disable tests setting the `-DBUILD_TEST=OFF` during the building.
All the test are performed using the [`Catch2`](https://github.com/catchorg/Catch2/) (v2.11.0) library.

The test scripts can be found [here](https://github.com/Nico-Curti/rFBP/blob/master/test).

The Python version of the package is also tested using [`pytest`](https://docs.pytest.org/en/latest/).
To install the package in development mode you need to add also this requirement:

* pytest == 3.0.7

The full list of python test scripts can be found [here](https://github.com/Nico-Curti/rFBP/blob/master/ReplicatedFocusingBeliefPropagation/rfbp/test)

## Table of contents

Description of the folders related to the `C++` version.

| **Directory**  |  **Description** |
|:--------------:|:-----------------|
| [example](https://github.com/Nico-Curti/rFBP/blob/master/example) | List of example usages for the C++ version of the code. In [train_main.cpp](https://github.com/Nico-Curti/rFBP/blob/master/example/train_main.cpp) we show how to build and train a C++ model and in [test_main.cpp](https://github.com/Nico-Curti/rFBP/blob/master/example/test_main.cpp) how to use this model to perform a prediction. |
| [hpp](https://github.com/Nico-Curti/rFBP/blob/master/hpp)         | Implementation of the C++ template functions and objects used in the `rFBP` library |
| [include](https://github.com/Nico-Curti/rFBP/blob/master/include) | Definition of the C++ function and objects used in the `rFBP` library |
| [src](https://github.com/Nico-Curti/rFBP/blob/master/src)         | Implementation of the C++ functions and objects used in the `rFBP` library |
| [test](https://github.com/Nico-Curti/rFBP/blob/master/test)       | Repository of tests for the C++ codes |

Description of the folders related to the `Python` version (base directory `ReplicatedFocusingBeliefPropagation`).

| **Directory**  |  **Description** |
|:--------------:|:-----------------|
| [example](https://github.com/Nico-Curti/rFBP/blob/master/ReplicatedFocusingBeliefPropagation/example) | `Python` version of the `C++` examples. In [overall_example.py](https://github.com/Nico-Curti/rFBP/blob/master/ReplicatedFocusingBeliefPropagation/example/overall_example.py) a full example (train + test) is showed using random patten. |
| [lib](https://github.com/Nico-Curti/rFBP/blob/master/ReplicatedFocusingBeliefPropagation/lib)         | List of `Cython` definition files |
| [source](https://github.com/Nico-Curti/rFBP/blob/master/ReplicatedFocusingBeliefPropagation/source)   | List of `Cython` implementation objects |
| [rfbp](https://github.com/Nico-Curti/rFBP/blob/master/ReplicatedFocusingBeliefPropagation/rfbp)       | List of `Python` wraps |
| [rfbp/test](https://github.com/Nico-Curti/rFBP/blob/master/ReplicatedFocusingBeliefPropagation/rfbp/test) | List of test scripts for the `Python` wraps |

## Contribution

Any contribution is more than welcome :heart:. Just fill an [issue](https://github.com/Nico-Curti/rFBP/blob/master/ISSUE_TEMPLATE.md) or a [pull request](https://github.com/Nico-Curti/rFBP/blob/master/PULL_REQUEST_TEMPLATE.md) and we will check ASAP!

See [here](https://github.com/Nico-Curti/rFBP/blob/master/CONTRIBUTING.md) for further informations about how to contribute with this project.

## References

<blockquote>1- D. Dall'Olio, N. Curti, G. Castellani, A. Bazzani, D. Remondini. "Classification of Genome Wide Association data by Belief Propagation Neural network", CCS Italy, 2019. </blockquote>

<blockquote>2- C. Baldassi, C. Borgs, J. T. Chayes, A. Ingrosso, C. Lucibello, L. Saglietti, and R. Zecchina. "Unreasonable effectiveness of learning neural networks: From accessible states and robust ensembles to basic algorithmic schemes", Proceedings of the National Academy of Sciences, 113(48):E7655-E7662, 2016. </blockquote>

<blockquote>3- C. Baldassi, A. Braunstein, N. Brunel, R. Zecchina. "Efficient supervised learning in networks with binary synapses", Proceedings of the National Academy of Sciences, 104(26):11079-11084, 2007. </blockquote>

<blockquote>4- A., Braunstein, R. Zecchina. "Learning by message passing in networks of discrete synapses". Physical Review Letters 96(3), 2006. </blockquote>

<blockquote>5- C. Baldassi, F. Gerace, C. Lucibello, L. Saglietti, R. Zecchina. "Learning may need only a few bits of synaptic precision", Physical Review E, 93, 2016 </blockquote>

<blockquote>6- A. Blum, R. L. Rivest. "Training a 3-node neural network is NP-complete", Neural Networks, 1992 </blockquote>

<blockquote>7- W. Krauth, M. Mezard. "Storage capacity of memory networks with binary coupling", Journal of Physics (France), 1989 </blockquote>

<blockquote>8- H. Huang, Y. Kabashima. "Origin of the computational hardness for learning with binary synapses", Physical Review E - Statistical, Nonlinear, and Soft Matter Physics, 2014 </blockquote>

<blockquote>9- C. Baldassi, A. Ingrosso, C. Lucibello, L. Saglietti, R. Zecchina. "Local entropy as a measure for sampling solutions in constraint satisfaction problems", Journal of Statistical Mechanics: Theory and Experiment, 2016 </blockquote>

<blockquote>10- R. Monasson, R. Zecchina. "Learning and Generalization Theories of Large Committee Machines", Modern Physics Letters B, 1995 </blockquote>

<blockquote>11- R. Monasson, R. Zecchina. "Weight space structure and internal representations: A direct approach to learning and generalization in multilayer neural networks", Physical Review Letters, 1995 </blockquote>

<blockquote>12- C. Baldassi, A. Braunstein. "A Max-Sum algorithm for training discrete neural networks", Journal of Statistical Mechanics: Theory and Experiment, 2015 </blockquote>

<blockquote>13- G. Parisi. "Mean field theory of spin glasses: statics and dynamics", arXiv, 2007 </blockquote>

<blockquote>14- L. Dall'Asta, A. Ramezanpour, R. Zecchina. "Entropy landscape and non-Gibbs solutions in constraint satisfaction problem", Physical Review E, 2008 </blockquote>

<blockquote>15- M. Mézard, A. Montanari. "Information, Physics and Computation", Oxford Graduate Texts, 2009 </blockquote>

<blockquote>16- C. Baldassi, A. Ingrosso, C. Lucibello, L. Saglietti, R. Zecchina. "Subdominant Dense Clusters Allow for Simple Learning and High Computational Performance in Neural Networks with Discrete Synapses", Physical Review Letters, 2015 </blockquote>

## Authors

* <img src="https://avatars0.githubusercontent.com/u/24650975?s=400&v=4" width="25px"> **Nico Curti** [git](https://github.com/Nico-Curti), [unibo](https://www.unibo.it/sitoweb/nico.curti2)
* <img src="https://avatars3.githubusercontent.com/u/23407684?s=400&v=4" width="25px"> **Daniele Dall'Olio** [git](https://github.com/DanieleDallOlio), [unibo](https://www.unibo.it/sitoweb/daniele.dallolio)
* <img src="https://avatars2.githubusercontent.com/u/25343321?s=400&v=4" width="25px"> **Daniel Remondini** [git](https://github.com/dremondini), [unibo](https://www.unibo.it/sitoweb/daniel.remondini)
* <img src="https://www.unibo.it/uniboweb/utils/UserImage.aspx?IdAnagrafica=236217&IdFoto=bf094429" width="25px"> **Gastone Castellani** [unibo](https://www.unibo.it/sitoweb/gastone.castellani)
* <img src="https://avatars2.githubusercontent.com/u/1419337?s=400&v=4" width="25px;"/> **Enrico Giampieri** [git](https://github.com/EnricoGiampieri), [unibo](https://www.unibo.it/sitoweb/enrico.giampieri)

See also the list of [contributors](https://github.com/Nico-Curti/rFBP/contributors) [![GitHub contributors](https://img.shields.io/github/contributors/Nico-Curti/rFBP.svg?style=plastic)](https://github.com/Nico-Curti/rFBP/graphs/contributors/) who participated in this project.

## License

The `rFBP` package is licensed under the MIT "Expat" License. [![License](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/Nico-Curti/rFBP/blob/master/LICENSE.md)

## Acknowledgments

Thanks goes to all contributors of this project.

We thank also the author(s) of [Catch2](https://github.com/catchorg/Catch2) library: we have used it in the testing procedure of our C++ version and it is amazing!

## Citation

If you have found `rFBP` helpful in your research, please consider citing the paper

```BibTeX
@misc{DallOlioCCS19,
  author = {Dall'Olio, Daniele and Curti, Nico and Castellani, Gastone and Bazzani, Armando and Remondini, Daniel},
  title = {Classification of Genome Wide Association data by Belief Propagation Neural network},
  year = {2019},
  conference = {Conference of Complex System}
}
```

or just this project repository

```BibTeX
@misc{ReplicatedFocusingBeliefPropagation,
  author = {Curti, Nico and Dall'Olio, Daniele and Giampieri, Enrico},
  title = {Replicated Focusing Belief Propagation},
  year = {2019},
  publisher = {GitHub},
  howpublished = {\url{https://github.com/Nico-Curti/rFBP}},
}
```
