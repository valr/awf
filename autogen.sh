#!/bin/sh

# Reconfigure all
autoreconf -f -i

# Check minimum required version
# if which gtkdoc-depscan &> /dev/null; then
#   echo "Minimum required version:"
#   gtkdoc-depscan --book=gtk * | cut -d ' ' -f 2  | sort | uniq
# fi
