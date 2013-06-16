#!/bin/sh
pwd_path=`pwd`

DIRECTORY=SpiceExtract-build

export BUILD_ROOT=$pwd_path/../$DIRECTORY

echo "BUILD_ROOT = $BUILD_ROOT"

if [ -d "$BUILD_ROOT" ]; then

  qtcreator CMakeLists.txt&

else 
  mkdir $BUILD_ROOT
  qtcreator CMakeLists.txt&
fi
