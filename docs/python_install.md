## Python Installation

First of all ensure that a right Python version is installed (Python >= 3.5 is required).
The [Anaconda/Miniconda](https://www.anaconda.com/) python version is recomended.

Download the project or the latest release:

```bash
git clone https://github.com/Nico-Curti/rFBP
cd rFBP
```

### Installing prerequisites

To install the prerequisites type:

```bash
pip install -r ./requirements.txt
```

### Installation from sources

In the `rFBP` directory execute:

```bash
python setup.py install
```

or for installing in development mode:

```bash
python setup.py develop --user
```

### Installation using pip

The latest release of the `rFBP` package can be installed using `pip`

```bash
pip install ReplicatedFocusingBeliefPropagation
```

The installation via `pip` requires to pre-install the `Cython` and `Numpy` packages, thus make sure to pre-install them!