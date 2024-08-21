#!/bin/bash

# -S <path-to-source>          = Explicitly specify a source directory
# -B <path-to-build>           = Explicitly specify a build directory
cmake                                                                 \
    -S .                                                              \
    -B project/build                                                  \
    -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake  \
    -DVCPKG_TARGET_TRIPLET=x64-linux                                  \
    -DVCPKG_HOST_TRIPLET=x64-linux

