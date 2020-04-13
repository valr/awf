# AWF - A Widget Factory (Extended)

Work in progress.

## About

A widget factory is a theme preview application for gtk2, gtk3 and gtk4. It displays the various widget types provided by gtk2/gtk3/gtk4 in a single window allowing to see the visual effect of the applied theme.

## Installation

It require *gtk2 (2.24+)* or *gtk3 (3.0+ including 3.24)* or *gtk4 (3.98+)*, and *glib (2.28+)*.

* From source: `apt install dh-autoreconf libgtk2.0-dev libgtk-3-dev libgtk-4-dev` then: `./build.sh`

## Screenshots

* The options menu allow to run some standard dialogs.
* The plus toolbar button show/hide: arrows on notebooks, text on progress bars and scales, marks on scales.
* The refresh toolbar button and the refresh menuitem allow to reload the current theme.
* Some options are available from command line (read -h).

![A widget factory - Gtk2](images/gtk2.png?raw=true)
![A widget factory - Gtk3](images/gtk3.png?raw=true)
![A widget factory - Gtk4](images/gtk4.png?raw=true)

## SIGHUP

To auto reload theme on files change, you can use [entr](https://github.com/clibs/entr).
```
apt install entr
ls ~/.themes/yourtheme/gtk-3.0/*.css | entr killall -s SIGHUP awf-gtk3
```

## Copyright and Credits

A widget factory is provided under the terms of the GNU GPLv3 license.

* Created by Valère Monseur (valere dot monseur at ymail dot com)
* Updated by Fabrice Creuzot (code hat luigifab dot fr)
* Thanks to Ottoman Kent (for testing in ubuntu and bug report)
* Thanks to Dwight Engen (for rpm specification file: awf.spec.in)
* Thanks to Josef Radinger (for sorted menus, text in progress bars, treeviews with(out) scrollbars)