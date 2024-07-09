# DOOM WAD World Tool

## How to build???

### Linux

`git clone --recursive https://github.com/ultimate-vegan/dwwtool.git`

`cd dwwtool`

`mkdir build && cd build`

`qmake -o Makefile ../dwwtool.pro && make`

### Windows

idk lol install linux

## Dependencies

### Arch Linux

`pacman -S qt5-base boost`

### Other OSes

idk lol install arch

## What else???

right now this is just a really shitty file browser and the config generation doesn't fully work. if you restart it after setting a WAD directory it should work.

the end goal is for it to display information about doom wads, be able to execute commands for recording demos, generate youtube video descriptions, and other stuff like that. hopefully it can be useful both for me and contributors to the channel.
