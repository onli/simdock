# Simdock

This is simdock, an eye-candy deskbar for Linux.

![simdock example](https://lh5.googleusercontent.com/-2a1A0WrrDzo/ThWuhAmT7OI/AAAAAAAABtI/5KGx3Ev2ErY/s800/simdock.jpg)

## Features

 * Starters in the dock keep track of launched applications, enabling minimizing, raising and cycling through its windows
 * Transparent background without compositing, which still gets automatically updated when the background changes
 * Fluid animations – icons get bigger when hovered and shortly transparent when clicked
 * Configurable size and icon placement

## Install

Currently, simdock has to be compiled manually. The main dependencies are wxWidgets and GTK3.

1. Install dependencies.

    Under Void Linux:

        sudo xbps-install pkg-config wxWidgets-gtk3-devel librsvg-devel xcb-ewmh libwnck-devel GConf-devel make gcc

1. Type `make` to compile
1. Type `sudo make install` to install

## Start

Start with `simdock`


## Original ##
http://sourceforge.net/projects/simdock
