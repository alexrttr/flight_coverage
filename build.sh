#!/bin/bash

cd "$(dirname "$0")"

rm -rf build
mkdir -p build && cd build
mkdir -p bin

cmake .. -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="$(pwd)/bin"
make VERBOSE=1

