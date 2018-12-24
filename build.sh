#!/bin/bash

if [[ "$OSTYPE" == "darwin"* ]]; then
  export CC="/usr/local/bin/gcc"
  export CXX="/usr/local/bin/g++"
  #export CC="/usr/local/Cellar/llvm/7.0.0_1/bin/clang"
  #export CXX="/usr/local/Cellar/llvm/7.0.0_1/bin/clang++"
  #export OPENMP_LIBRARIES="/usr/local/Cellar/llvm/7.0.0_1/lib"
  #export OPENMP_INCLUDES="/usr/local/Cellar/llvm/7.0.0_1/include"
fi

#rm -rf build
mkdir -p build
cd build

##sudo apt-get install ninja-build
cmake -G "Ninja" "-DCMAKE_BUILD_TYPE=Release" ..
cmake --build .
cd ..
