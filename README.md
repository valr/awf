AWF - A Widget Factory
======================

Description
-----------

A widget factory is a theme preview application for gtk2 and gtk3.
It displays the various widget types provided by gtk2/gtk3 in a single window allowing to see the visual effect of the applied theme.

Features
--------

  * Display widget types available in gtk2/gtk3 (remark: option menu widget type only exists in gtk2 and switch widget type only exists in gtk3)
  * Menus to select the themes available either at system level or user level
  * Toolbar button to start the other gtk version of the tool
  * Toolbar button to refresh the current theme

Installation
------------

  * Ubuntu Oneiric

    sudo add-apt-repository [ppa:valere-monseur/valr](https://launchpad.net/~valere-monseur/+archive/valr/+packages)  
    sudo apt-get update  
    sudo apt-get install awf

  * Archlinux

    https://aur.archlinux.org/packages.php?ID=54550

  * From source

    ./autogen.sh  
    ./configure  
    make  
    make install

Dependencies
------------

  * gtk2 version 2.24
  * gtk3

Screenshots
-----------

![A widget factory](https://github.com/valr/awf/raw/gh-pages/awf-gtk2.jpg)

![A widget factory](https://github.com/valr/awf/raw/gh-pages/awf-gtk3.jpg)

License
-------

  A widget factory is provided under the terms of the GNU GPLv3 license.
  See the COPYING file for details.

Author
------

  Valère Monseur (valere dot monseur at ymail dot com)

Thanks to
---------

  * Ottoman Kent (for testing in ubuntu and bug report)
