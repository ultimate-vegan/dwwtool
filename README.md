# DOOM WAD World Tool

## Description

this is a tool meant to be used by myself and contributors to my youtube channel, [doom wad world](https://www.youtube.com/channel/UCGwgSggXMjKQkp3_vup4EtA). it's primarily a method for me to learn some coding skills and waste some time on something useful. so be warned, there will be bad coding practices and amateur development within. it currently only supports linux because that's what I use.

## How to build???

### Linux

`git clone --recursive https://github.com/ultimate-vegan/dwwtool.git`

`cd dwwtool`

`mkdir build && cd build`

`qmake -o Makefile ../dwwtool.pro && make`

### Windows

idk lol install linux

certain features definitely won't work on windows due to them executing commands in a linux terminal, and there is currently no code for windows support

## Dependencies

### Arch Linux

`pacman -S qt5-base boost`

### Other OSes

idk lol install arch

## Planned Features

- Browse directories with DOOM WADs in them (sort of finished)

- Display information and visuals about DOOM .WAD and .deh files, including:
    
    - Number of levels and which levels are replaced

    - Map layouts and contents (i.e. number of monsters, items, secrets)

    - Modified sounds and graphics

    - For .deh files, list the things they modify

    - Probably some other things I can't think of

- Display contents of .txt files (done)

- Display image files (done)

- Interface with the DOOM WAD World spreadsheet and youtube channel to check if WADs have been played

- Generate youtube video descriptions based on information found from file metadata and WAD readme contents

- Run user specified commands to expedite the demo recording process, and (for me) the video file creation process

- Interface with Discord/Google Forms to expedite the demo submission process

- A UI that doesn't look terrible

- Multi-platform support including Windows
