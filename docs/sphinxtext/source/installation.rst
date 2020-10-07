Installation guide
==================

C++ supported compilers:

|gcc version|

|clang version|

|msvc version|

The `rFBP` project is written in `C++` using a large amount of c++17 features.
To enlarge the usability of our package we provide also a retro-compatibility of all the c++17 modules reaching an usability (tested) of our code from gcc 4.8.5+.
The package installation can be performed via `CMake` or `Makefile`.

If you are using the `CMake` (recommended) installer the maximum version of C++ standard is automatic detected.
The `CMake` installer provides also the export of the library: after the installation you can use this library into other `CMake` projects using a simple `find_package` function.
The exported `CMake` library (`rFBP::rfbp`) is installed in the `share/rFBP` directory of the current project and the relative header files are available in the `rFBP_INCLUDE_DIR` variable.

The `CMake` installer provides also a `rFBP.pc`, useful if you want link to the `rFBP` using `pkg-config`.

You can also use the `rFBP` package in `Python` using the `Cython` wrap provided inside this project.
The only requirements are the following:

* numpy >= 1.15
* cython >= 0.29
* scipy >= 1.2.1
* scikit-learn >= 0.20.3
* requests >= 2.22.0

The `Cython` version can be built and installed via `CMake` enabling the `-DPYWRAP` variable.
The `Python` wrap guarantees also a good integration with the other common Machine Learning tools provided by `scikit-learn` `Python` package; in this way you can use the `rFBP` algorithm as an equivalent alternative also in other pipelines.
Like other Machine Learning algorithm also the `rFBP` one depends on many parameters, i.e its hyper-parameters, which has to be tuned according to the given problem.
The `Python` wrap of the library was written according to `scikit-optimize` `Python` package to allow an easy hyper-parameters optimization using the already implemented classical methods.


.. |gcc version| image:: https://img.shields.io/badge/gcc-4.8.5%20|%204.9.*%20|%205.*%20|%206.*%20|%207.*%20|%208.*%20|%209.*-yellow.svg
.. |clang version| image:: https://img.shields.io/badge/clang-3.6%20|3.9%20|5.*%20|%206.*%20|%207.*%20|-red.svg
.. |msvc version| image:: https://img.shields.io/badge/msvc-vs2017%20x86%20|%20vs2017%20x64|%20vs2019%20x86%20|%20vs2019%20x64-blue.svg
.. _CMake: https://github.com/Nico-Curti/rFBP/blob/master/CMakeLists.txt
.. _Makefile: https://github.com/Nico-Curti/rFBP/blob/master/Makefile

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   CMake
   Python
