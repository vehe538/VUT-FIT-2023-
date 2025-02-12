#!/bin/bash

if [ -d "build" ]; then
    rm -rf build
fi

mkdir build
cd build
cmake ..

CPU_CORES=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
make -j $CPU_CORES