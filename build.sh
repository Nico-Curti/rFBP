#!/bin/bash

if [[ "$OSTYPE" == "darwin"* ]]; then
  export CC="/usr/local/bin/gcc"
  export CXX="/usr/local/bin/g++"
  #export CC="/usr/local/Cellar/llvm/7.0.0_1/bin/clang"
  #export CXX="/usr/local/Cellar/llvm/7.0.0_1/bin/clang++"
  #export OPENMP_LIBRARIES="/usr/local/Cellar/llvm/7.0.0_1/lib"
  #export OPENMP_INCLUDES="/usr/local/Cellar/llvm/7.0.0_1/include"
else
  export CC="/usr/bin/gcc-8"
  export CXX="/usr/bin/g++-8"
fi

rm -rf build
mkdir -p build
cd build

##sudo apt-get install ninja-build
cmake -G "Ninja" "-DOMP=ON" "-DCMAKE_BUILD_TYPE=Release" ..
make -j4 install
cd ..
