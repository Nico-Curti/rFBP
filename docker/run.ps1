#!/usr/bin/env pwsh

# Build docker image
docker.exe build .

# Run docker image
docker.exe run --rm -it --name rFBP
