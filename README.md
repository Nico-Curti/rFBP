| **Authors**  | **Project** | **Documentation**                                                                   | **Build Status**              |
|:------------:|:-----------:|:-----------------------------------------------------------------------------------:|:-----------------------------:|
<<<<<<< HEAD
|   N. Curti   |    rFBP    | [![docs](https://img.shields.io/readthedocs/:packageName/:version.svg?style=plastic)](https://nico-curti.github.io/rFBP/) | **Linux/MacOS** : [![Travis](https://travis-ci.com/Nico-Curti/rFBP.svg?token=7QqsqaQiuDHSyGDT3xek&branch=master)](https://travis-ci.com/Nico-Curti/rFBP) <br/> **Windows** : *miss* |

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

## Prerequisites

The project is written in c++ language with the support of c++17 standard (*make sure that your c++ compiler support it!*). If you want enable the OpenMP support (*no particular version is required*) compile the library with `-DOMP=ON`.

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

See also the list of [contributors](https://github.com/Nico-Curti/rFBP/contributors) [![GitHub contributors](https://img.shields.io/github/contributors/Nico-Curti/rFBP.svg?style=plastic)](https://github.com/Nico-Curti/rFBP/graphs/contributors/) who partecipated in this project.

## License

NO LICENCEs are available.

### Acknowledgement

Thanks goes to all contributors of this project:

[<img src="https://avatars3.githubusercontent.com/u/23407684?s=400&v=4" width="100px;"/><br /><sub><b>Daniele Dall'Olio</b></sub>](https://github.com/DanieleDallOlio)<br />
