# Simdock

This is simdock, an eye-candy deskbar for Linux.

![simdock example](https://lh5.googleusercontent.com/-2a1A0WrrDzo/ThWuhAmT7OI/AAAAAAAABtI/5KGx3Ev2ErY/s800/simdock.jpg)

## Features

 * Starters in the dock keep track of launched applications, enabling minimizing, raising and cycling through its windows
 * Transparent background without compositing, which still gets automatically updated when the background changes
 * Fluid animations – icons get bigger when hovered and shortly transparent when clicked
 * Configurable size and icon placement

## Install

### Ubuntu

On launchpad, there is a [PPA with daily builds](https://launchpad.net/~onli/+archive/simdock). Add it with

    sudo add-apt-repository ppa:onli/simdock

and install simdock

    sudo apt-get update
    sudo apt-get install simdock
    
### Gentoo/Funtoo

Use the [overlay](https://github.com/onli/overlay ) with layman

    layman -o https://raw.github.com/onli/overlay/master/repositories.xml -f -a onli
    
and install with emerge

    emerge simdock


### Manually

1. Install dependencies. Under Ubuntu:

        sudo apt-get install pkg-config libglib2.0-dev libgconf2-dev libgtk2.0-dev libwnck-dev libwxgtk3.0-dev libxcb1-dev libxcb-ewmh-dev xcb-proto librsvg2-dev

1. Type `make` to compile
1. Type `sudo make install` to install

## Start

Start with `simdock`


## Original ##
http://sourceforge.net/projects/simdock
