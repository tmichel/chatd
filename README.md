# chatd

For english please [scroll down](#overview).

Linux programozás házi feladat, 2013 ősz, BME VIK.

## Áttekintés

Egyszerű chat szerver C-ben. Részletes leírás a `SPECS.md` fájlban található.

## Overview

This is a home assignment for the [Programming of Linux](https://www.vik.bme.hu/kepzes/targyak/VIAUJV57)
course at Budapest University of Technology and Economics.

Autumn, 2013.

This is a simple chat server written in C. Details can be found in the `SPECS.md`
in Hungarian.

## Install and usage

The only dependency is SQLite3. It can be installed from the package manager on
most Linux distributions.

When you got all the dependecies just run

    $ make
    $ ./chatd # listens on the port 3222

There is no client at the moment, but you can use telnet for this.

    $ telnet localhost 3222

The protcol details can be found in the `PROTOCOL.md` file (it is in Hungarian).
