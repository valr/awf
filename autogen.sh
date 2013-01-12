#!/bin/bash

# Ensure README file exists
if test ! -e README; then
  cp README.md README
fi

# Reconfigure all
autoreconf -f -i

# Check minimum required gtk2 version
if which gtkdoc-depscan &> /dev/null; then
  echo "Minimum required version:"
  gtkdoc-depscan --book=gtk2 * | cut -d ' ' -f 2  | sort | uniq
fi

# Check minimum required gtk3 version
if which gtkdoc-depscan &> /dev/null; then
  echo "Minimum required version:"
  gtkdoc-depscan --book=gtk3 * | cut -d ' ' -f 2  | sort | uniq
fi
