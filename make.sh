#!/usr/bin/env bash

cd wheelcc/
find . -name "*.o" -type f -delete
if [ ${?} -ne 0 ]; then exit 1; fi
rm -r src/ include/ lib/ build/build_cache/ build/build.sh bin/pkgname.conf
if [ ${?} -ne 0 ]; then exit 1; fi

cp -r ../src ../include ../lib .
if [ ${?} -ne 0 ]; then exit 1; fi
cp ../build.sh build/
if [ ${?} -ne 0 ]; then exit 1; fi
cp ../pkgname.conf bin/
if [ ${?} -ne 0 ]; then exit 1; fi

cd bin/
./make.sh
if [ ${?} -ne 0 ]; then exit 1; fi
