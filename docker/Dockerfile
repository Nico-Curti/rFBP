FROM ubuntu:16.04

LABEL maintainer="nico.curti2@unibo.it"

RUN apt-get update

## Create work directory
RUN mkdir -p /usr/src/workspace
WORKDIR /usr/src/workspace

## Installing common dependencies
RUN echo "*** Installing common dependecies ***"
WORKDIR /usr/src/workspace
RUN apt-get update
RUN apt-get install -y apt-utils --no-install-recommends

## Installing g++-8
RUN echo "*** Installing g++-8 ***"
WORKDIR /usr/src/workspace
RUN apt-get dist-upgrade -y
RUN apt-get install -y software-properties-common --no-install-recommends
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update
RUN apt-get install -y --no-install-recommends gcc-8 g++-8
RUN apt-get autoremove --purge -y
RUN apt-get autoclean -y

# Setting g++-8 as default compiler
RUN echo "*** Setting g++ 8 as default compiler ***"
ENV CC /usr/bin/gcc-8
ENV CXX /usr/bin/g++-8

# Installing CMake
RUN echo "*** Installing CMake ***"
WORKDIR /usr/src/workspace
RUN wget --no-check-certificate https://cmake.org/files/v3.13/cmake-3.13.1-Linux-x86_64.tar.gz
RUN tar -xzf cmake-3.13.1-Linux-x86_64.tar.gz
ENV PATH=/usr/src/workspace/cmake-3.13.1-Linux-x86_64/bin:$PATH

# Installing rFBP Library
RUN echo "*** Installing rFBP Library ***"
WORKDIR /usr/src/workspace
RUN git clone https://github.com/Nico-Curti/rFBP
RUN cmake "-DCMAKE_BUILD_TYPE=Release" ..
RUN cmake --build . --target install
