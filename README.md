AWF - A Widget Factory
======================

Description
-----------

A widget factory is a theme preview application for gtk2 and gtk3.
It displays the various widget types provided by gtk2/gtk3 in a single window allowing to see the visual effect of the applied theme.

Features
--------

  * Display widget types available in gtk2/gtk3
  * Menus to select the themes available either at system level or user level
  * Toolbar button and menu item to refresh the current theme
  * Toolbar button to show or hide notebook tabs arrows
  * Refresh the current theme on SIGHUP

Installation
------------

It require *gtk2* version 2.24 and *gtk3*.

  * Ubuntu
```
sudo apt-add-repository ppa:flexiondotorg/awf
sudo apt update
sudo apt install awf
```

  * Archlinux

    https://aur.archlinux.org/packages/awf-git/

  * RPM based distro

    There is no pre-built package (yet) but you can generate RPM like this:

```
./autogen.sh
./configure
make rpm
```

  * From source

```
apt install dh-autoreconf libgtk2.0-dev libgtk-3-dev
./autogen.sh
./configure
make
make install
```

Screenshots
-----------

![A widget factory - Gtk2](images/gtk2.png?raw=true)

![A widget factory - Gtk2](images/gtk3.png?raw=true)

SIGHUP
------

  To auto reload theme on files change, you can use [entr](https://github.com/clibs/entr).
```
apt install entr
ls ~/.themes/yourtheme/gtk-3.0/*.css | entr killall -s SIGHUP awf-gtk3
```

License
-------

  A widget factory is provided under the terms of the GNU GPLv3 license.
  See the COPYING file for details.

Authors
-------

  * Valère Monseur (valere dot monseur at ymail dot com)
  * Fabrice Creuzot (code hat luigifab dot fr)

Thanks to
---------

  * Ottoman Kent (for testing in ubuntu and bug report)
  * Dwight Engen (for rpm specification file: awf.spec.in)
  * Josef Radinger (for sorted menus, text in progress bars, treeviews with(out) scrollbars)
