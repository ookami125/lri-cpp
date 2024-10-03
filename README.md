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