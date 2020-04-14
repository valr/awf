#!/bin/bash

# copy to a tmp directory
mkdir builder builder/src

touch builder/ChangeLog
touch builder/NEWS
touch builder/AUTHORS
touch builder/README
cp LICENSE         builder/COPYING
cp configure.ac    builder/
cp Makefile.am     builder/
cp src/Makefile.am builder/src/
cp src/awf.c       builder/src/

cd builder/

# reconfigure all
autoreconf -f -i

# build
./configure
make > ../log

# final
cp src/awf-gtk* ..

cd ..
ls -altrh awf-gtk4 awf-gtk3 awf-gtk2
rm -rf builder/
