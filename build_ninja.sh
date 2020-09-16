#!/bin/bash

# Check input arguments.
if [[ ! $1 =~ ^(Debug|Release)$ ]]; then
  echo "Usage:"
  echo "$(basename "$0") <Debug|Release>"
  exit
fi

BUILD_TYPE=$1

# Build.
cmake -S "." -B "build/$BUILD_TYPE" -G "Ninja" -DCMAKE_BUILD_TYPE:STRING=$BUILD_TYPE -DBUILD_SHARED_LIBS:BOOL=ON -DBUILD_TESTS:BOOL=OFF -DCMAKE_INSTALL_PREFIX:PATH="$(pwd)/build/$BUILD_TYPE/install"
cmake --build "build/$BUILD_TYPE" --config $BUILD_TYPE --parallel 4
cmake --install "build/$BUILD_TYPE" --config $BUILD_TYPE --strip
