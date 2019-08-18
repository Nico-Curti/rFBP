#!/usr/bin/env bash

# Build docker image
docker build .

# Run docker image
docker run --rm -it --name rFBP
