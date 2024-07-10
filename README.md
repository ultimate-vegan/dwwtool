# DOOM WAD World Tool

## How to build???

### Linux

`git clone --recursive https://github.com/ultimate-vegan/dwwtool.git`

`cd dwwtool`

`mkdir build && cd build`

`qmake -o Makefile ../dwwtool.pro && make`

### Windows

idk lol install linux

i have no idea if this program will even work on windows right now, i don't have a windows installation to test it on

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

- (Possibly) interface with Discord/Google Forms to expedite the demo submission process

- A UI that doesn't look terrible
