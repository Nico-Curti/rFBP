| **Authors**  | **Project** | **Documentation**                                                                   | **Build Status**              |
|:------------:|:-----------:|:-----------------------------------------------------------------------------------:|:-----------------------------:|
| [**N. Curti**](https://github.com/Nico-Curti) <br/> [**D. Dall'Olio**](https://github.com/DanieleDallOlio)   |  **rFBP**  | [![docs](https://img.shields.io/readthedocs/:packageName/:version.svg?style=plastic)](https://nico-curti.github.io/rFBP/) | **Linux/MacOS** : [![Travis](https://travis-ci.com/Nico-Curti/rFBP.svg?token=7QqsqaQiuDHSyGDT3xek&branch=master)](https://travis-ci.com/Nico-Curti/rFBP) <br/> **Windows** : *miss* |

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

Optimization and extension of the [**Replicated Focusing Belief Propagation**](https://github.com/carlobaldassi/BinaryCommitteeMachineFBP.jl).

1. [Prerequisites](#prerequisites)
2. [Installation](#installation)
3. [Authors](#authors)
4. [License](#license)
5. [Acknowledgments](#acknowledgments)
6. [Citation](#citation)

## Prerequisites

The project is written in c++ language with the support of c++17 standard (*make sure that your c++ compiler support it!*). If you want enable the OpenMP support (*4.5 version is required*) compile the library with `-DOMP=ON`.

If you use MagT configuration, please download the `atanherf coefficients` file before running. You can find a downloader script inside the [scripts](https://github.com/Nico-Curti/rFBP/tree/master/scripts) folder. Enter in that folder and just run `python dowload_atanherf.py`.

## Installation

To build the executables, clone the repo and then type

```
make
```

for the complete list of available examples, or

```
make main
```

to compile the main example.

You can also build with CMake or easier with the [build.sh](https://github.com/Nico-Curti/rFBP/blob/master/build.sh) file in Linux/MacOS and [build.ps1](https://github.com/Nico-Curti/rFBP/blob/master/build.ps1) in Windows.

## Authors

* **Nico Curti** [git](https://github.com/Nico-Curti), [unibo](https://www.unibo.it/sitoweb/nico.curti2)
* **Daniele Dall'Olio** [git](https://github.com/DanieleDallOlio)

See also the list of [contributors](https://github.com/Nico-Curti/rFBP/contributors) [![GitHub contributors](https://img.shields.io/github/contributors/Nico-Curti/rFBP.svg?style=plastic)](https://github.com/Nico-Curti/rFBP/graphs/contributors/) who participated in this project.

## License

The `rFBP` package is licensed under the MIT "Expat" License. [![License](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/Nico-Curti/rFBP/blob/master/LICENSE.md)

### Acknowledgment

Thanks goes to all contributors of this project.

### Citation

Please cite `rFBP` if you use it in your research.

```tex
@misc{ReplicatedFocusingBeliefPropagation,
  author = {Nico Curti, Daniele Dall'Olio},
  title = {{R}eplicated {F}ocusing {B}elief {P}ropagation},
  year = {2019},
  publisher = {GitHub},
  howpublished = {\url{https://github.com/Nico-Curti/rFBP}},
}
```
