#!/bin/sh

rm -rf ./build/
#mkdir ../$(date +%Y-%m-%d)
mkdir ../bkup

make clean
rm SpiceExtract
tar czvf ../bkup/SpiceExtract-$(date +%Y-%m-%d).tar.gz ../SpiceExtract/
#mv ../SpiceExtract.tar.gz ../$(date +%Y-%m-%d)
#rm -f -r ../SpiceParamExtractor/

