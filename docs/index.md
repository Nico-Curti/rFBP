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
To further improve the usage of our code, we propose also a `Python` wrap of the library with a full compatibility with the [`scikit-learn`](https://github.com/scikit-learn/scikit-learn) and [`scikit-optimize`](https://github.com/scikit-optimize/scikit-optimize) packages.

- [C++ Install](./cpp_install.md)
- [Python Install](./python_install.md)
- [Model description](./model.md)
- [Authors](./authors.md)
