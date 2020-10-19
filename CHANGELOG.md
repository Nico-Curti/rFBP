# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.3] - 2020-10-19

This is the official release for the JOSS paper [![JOSS](https://joss.theoj.org/papers/7643779111039dbc7776ff49d2a6b1b0/status.svg)](https://joss.theoj.org/papers/7643779111039dbc7776ff49d2a6b1b0).

### Added

- Add the [pyproject.toml](https://github.com/Nico-Curti/rFBP/blob/master/pyproject.toml) for the build requirements according to PEP-518.
- Add the Pypi badge for the latest releases.
- Add the Doxygen build via CMake.
- Upload the package to Zenodo.

### Fixed

- Fix bibliography and paper document
- Fix minor issues in the documentation

## [1.0.2] - 2020-10-12

### Added

- Doxygen documentation of the C++ APIs
- Sphinx documentation of the Python APIs
- CI for the documentation build

### Fixed

- python setup using pip, fixing dependencies and installation paths

## [1.0.0] - 2020-07-03

### Added

- First C++ version of the BinaryCommitteeMachineFBP code written in Julia
- Add Cython wrap for the Python support
- Add scikit-learn compatibility
- Add scorer support for the score evaluation
- Add old std compatibility in C++

### Changed

- Change the file organization in C++ modules
- Change magnetization formulas for computational performance improvements

### Fixed

- Add a complete Python support for the magnetization variables
- New performances visualization plots
