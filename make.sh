#!/usr/bin/env bash

sudo -v

cd wheelcc/
find . -name "*.o" -type f -delete; if [ ${?} -ne 0 ]; then exit 1; fi
if [ -d "src/" ]; then rm -r src/; fi; if [ ${?} -ne 0 ]; then exit 1; fi
if [ -d "include/" ]; then rm -r include/; fi; if [ ${?} -ne 0 ]; then exit 1; fi
if [ -d "lib/" ]; then rm -r lib/; fi; if [ ${?} -ne 0 ]; then exit 1; fi
if [ -d "build/build.sh/" ]; then rm -r build/build.sh/; fi; if [ ${?} -ne 0 ]; then exit 1; fi
if [ -f "build/build.sh" ]; then rm build/build.sh; fi; if [ ${?} -ne 0 ]; then exit 1; fi
if [ -f "bin/pkgname.cfg" ]; then rm bin/pkgname.cfg; fi; if [ ${?} -ne 0 ]; then exit 1; fi

cp -r ../src ../include ../lib .
if [ ${?} -ne 0 ]; then exit 1; fi
cp ../build.sh build/
if [ ${?} -ne 0 ]; then exit 1; fi
cp ../pkgname.cfg bin/
if [ ${?} -ne 0 ]; then exit 1; fi

cd bin/
./make.sh
if [ ${?} -ne 0 ]; then exit 1; fi

yes | ./install.sh
if [ ${?} -ne 0 ]; then exit 1; fi
