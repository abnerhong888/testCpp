#!/bin/bash
rm CMakePresets.json -f
ln -s ../CMakePresets.json CMakePresets.json

mkdir -p build

cd build

rm * -rf

cmake --preset=default ..

cmake --build .



