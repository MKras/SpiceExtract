#!/usr/bin/sh
rm -rf build
mkdir build
cd build
cmake CMakeLists.txt -G "Unix Makefiles" ../
cd ..
#pause
