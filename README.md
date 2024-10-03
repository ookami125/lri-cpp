# LRI-CPP
Basic program to extract image data from lri files.

## Usage
    Extract the contents of LRI files.
    Usage:
      lri_extractor [<options>] lri_file
      lri_extractor -h | --help
    
    Options:
      -h --help     Show this screen.
      -f --format   Output format [default: PGM] (PGM)
      -o --output   Output path [default: "buffers"]
      -d --debayer  Debayering mode [default: Interleaved] (None, Filter, Interleaved)

## Building
    $ git clone https://github.com/ookami125/lri-cpp
    $ cd lri-cpp
    $ cmake -B build -G "Ninja Multi-Config"
    $ cmake --build build --config Release

## Image stitching
A python file has been included that does some rudementary image stitching and white balancing.  
The basic way to use the program is just to run
>python stitch.py buffers/*.png

I however won't provide specific instructions for this as I mainly wrote it using chatgpt. I don't know everything required to make a python program work. The images should be converted to 8 bit pngs, I don't believe the the script supports 16 bit pngs. Command I used to do that is
>find buffers -type f -name '*.pgm' -exec magick {} -depth 8 {}.png \;

Good luck and have fun.