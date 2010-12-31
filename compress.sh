#!/bin/sh

mkdir ../$(date +%Y-%m-%d)

make clean
rm SpiceExtract
tar czvf ../SpiceExtract.tar.gz ../SpiceExtract/
mv ../SpiceExtract.tar.gz ../$(date +%Y-%m-%d)
#rm -f -r ../SpiceParamExtractor/

