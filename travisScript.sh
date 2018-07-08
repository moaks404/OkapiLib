#!/bin/sh

if "x$ARCH" = "xlinux"; then
    mkdir cmake-build-debug
fi

make
mkdir cmake-build-debug
cd cmake-build-debug
cmake -DCMAKE_BUILD_TYPE=Debug -G "CodeBlocks - Unix Makefiles" ..
cmake --build . --target OkapiLibV5 -- -j 2
./OkapiLibV5
