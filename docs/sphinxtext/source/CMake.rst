CMake C++ Installation
======================

We recommend to use `CMake` for the installation since it is the most automated way to reach your needs.
First of all make sure you have a sufficient version of `CMake` installed (3.9 minimum version required).
If you are working on a machine without root privileges and you need to upgrade your `CMake` version a valid solution to overcome your problems is provided shut_.

With a valid `CMake` version installed first of all clone the project as:

.. code-block:: bash

	git clone https://github.com/Nico-Curti/rFBP
	cd rFBP


The you can build the `rFBP` package with

.. code-block:: bash

	mkdir -p build
	cd build && cmake .. && cmake --build . --target install

or more easily

.. code-block:: bash

	./build.sh

if you are working on a Windows machine the right script to call is the `build.ps1`_.

.. note::
  If you want enable the OpenMP support (*4.5 version is required*) compile the library with `-DOMP=ON`.

.. note::
	If you want enable the Scorer support compile the library with `-DSCORER=ON`. If you want use a particular installation of the Scorer library or you have manually installed the library following the `README` instructions, we suggest to add the `-DScorer_DIR=/path/to/scorer/shared/scorer` in the command line.

.. note::
	If you want enable the Cython support compile the library with `-DPYWRAP=ON`. The Cython packages will be compiled and correctly positioned in the `rFBP` Python package **BUT** you need to run also the setup before use it.

.. note::
	If you use MagT configuration, please download the `atanherf coefficients` file before running any executable. You can find a downloader script inside the scripts_ folder. Enter in that folder and just run `python dowload_atanherf.py`.

.. _shut: https://github.com/Nico-Curti/Shut
.. _`build.ps1`: https://Nico-Curti/rFBP/blob/master/build.ps1
.. _scripts: https://github.com/Nico-Curti/rFBP/tree/master/scripts