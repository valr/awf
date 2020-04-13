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
cp src/gtk_empty.c builder/src/
cp src/gtk_empty.h builder/src/

cd builder/

# reconfigure all
autoreconf -f -i

# check minimum required gtk2 version
if which gtkdoc-depscan &> /dev/null; then
	echo "Minimum required version:"
	gtkdoc-depscan --book=gtk2 * | cut -d ' ' -f 2  | sort | uniq
fi

# Check minimum required gtk3 version
if which gtkdoc-depscan &> /dev/null; then
	echo "Minimum required version:"
	gtkdoc-depscan --book=gtk3 * | cut -d ' ' -f 2  | sort | uniq
fi

# Check minimum required gtk4 version
if which gtkdoc-depscan &> /dev/null; then
	echo "Minimum required version:"
	gtkdoc-depscan --book=gtk4 * | cut -d ' ' -f 2  | sort | uniq
fi

# build
./configure
make > ../log

# final
cp src/awf-gtk* ..

cd ..
ls -altrh awf-gtk4 awf-gtk3 awf-gtk2
rm -rf builder/
