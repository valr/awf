<h1 align="center">AWF - A Widget Factory</h1>
<img src="https://github.com/owl4ce/awf/raw/gh-pages/awf-gtk2-gtk3.png" alt="awf-gtk2-gtk3" align="right" width="400px">

## :pencil2: Description

A widget factory is a theme preview application for gtk2 and gtk3.
It displays the various widget types provided by gtk2/gtk3 in a single window allowing to see the visual effect of the applied theme.

## :pushpin: Status

**Warning**: it's quite some time I haven't looked at gtk3 to integrate the latest widgets, and I don't plan to work on this tool anymore.
AWF was indeed created a long time ago when neither gtk2 nor gtk3 were providing such a tool.  
As part of gtk3 there is now [gtk3-widget-factory](https://developer.gnome.org/gtk3/stable/gtk3-widget-factory.html) and gtk4 will provide [gtk4-widget-factory](https://developer.gnome.org/gtk4/stable/gtk4-widget-factory.html).

## :bookmark: Features

- Display widget types available in gtk2/gtk3 (remark: option menu widget type only exists in gtk2, switch and level bar widget types only exist in gtk3)
- Menus to select the themes available either at system level or user level
- Toolbar button to start the other gtk version of the tool
- Toolbar button to refresh the current theme (not working anymore in latest versions of gtk3)
- Refresh the current theme on SIGHUP

## :computer: Installation

<details open>
<summary><b>Ubuntu</b></summary>
 
```bash
sudo apt-add-repository ppa:flexiondotorg/awf  
sudo apt update  
sudo apt install awf
```

</details>

<details open>
<summary><b>Arch Linux</b></summary>

```bash
yay -S awf-git
```
https://aur.archlinux.org/packages/awf-git/

</details>

<details open>
<summary><b>RPM Based Distro</b></summary>

There is no pre-built package (yet) but you can generate RPM like this:
```bash
./autogen.sh  
./configure  
make rpm
```

</details>

<details open>
<summary><b>From Source</b></summary>

```bash
./autogen.sh  
./configure  
make  
make install
```

</details>

## :dvd: Dependencies

- gtk2 version 2.24
- gtk3

## :bookmark_tabs: License

A widget factory is provided under the terms of the GNU GPLv3 license.
See the [COPYING](./COPYING) file for details.

## :bust_in_silhouette: Author

Valère Monseur (valere dot monseur at ymail dot com)

## :heart: Thanks to

- Ottoman Kent (for testing in ubuntu and bug report)
- Dwight Engen (for rpm specification file: awf.spec.in)
- Josef Radinger (for sorted menus, text in progress bars, treeviews with(out) scrollbars)
