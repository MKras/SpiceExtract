#!/usr/bin/sh
rm -rf ./build
mkdir build
cd build
export BUILD_ROOT=`pwd`
#echo $BUILD_ROOT
cmake CMakeLists.txt -G "Unix Makefiles" ../ 
make
cd ..
#pause
