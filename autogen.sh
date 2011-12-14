#!/bin/sh

# Ensure README file exists
if test ! -e README; then
  cp README.md README
fi

# Reconfigure all
autoreconf -f -i

# Check minimum required gtk version
if which gtkdoc-depscan &> /dev/null; then
  echo "Minimum required version:"
  gtkdoc-depscan --book=gtk * | cut -d ' ' -f 2  | sort | uniq
fi
