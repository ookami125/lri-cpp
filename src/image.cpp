#include "image.h"
#include <algorithm>
#include <stdio.h>

Image::Image(size_t width, size_t height, uint8_t channels) :
    width(width), height(height), channels(channels)
{
    data.resize(width * height * channels);
}

Image::~Image() {
    data.clear();
}

int clamp(int val, int min, int max) {
    if(val < min) return min;
    if(val > max) return max;
    return val;
}

uint16_t Image::getColor(int x, int y, int c) {
    int xc = clamp(x, 0, width-1);
    int yc = clamp(y, 0, height-1);
    return data[(xc+yc*width)*3+c];
};

uint16_t Image::getColorOrMirror(int x, int y, int c) {
    const int w = width-1;
    const int h = height-1;
    // Stupid math to mirror the image
    x = w-abs(abs(x)%(w*2)-w);
    y = h-abs(abs(y)%(h*2)-h);
    return data[(x+y*width)*3+c];
};

void Image::setColor(int x, int y, int chan, uint16_t color) {
    int xc = clamp(x, 0, width-1);
    int yc = clamp(y, 0, height-1);
    data[(xc+yc*width)*3+chan] = color;
}

void Image::writeToFile(const char* path, ImageFileFormat format) {
    const uint16_t max = 1023;
    FILE* file = fopen(path, "wb");

    switch(format) {
        case ImageFileFormat::PGM: {

            if(channels == 1)
                fprintf(file, "P2\n");
            else if (channels == 3)
                fprintf(file, "P3\n");
            else {
                fclose(file);
                fprintf(stderr, "Error: invalid number of channels for PGM file format!");
                return;
            }

            fprintf(file, "%zu %zu\n%d\n", width, height, max);

            for(size_t y=0; y<height; ++y) {
                for(size_t x=0; x<width; ++x) {
                    uint16_t* pixel = &data[(x+y*width) * channels];
                    for(int channel=0; channel<channels; channel++) {
                        fprintf(file, "%d ", pixel[channel]);
                    }
                }
                fprintf(file, "\n");
            }
        } break;
    }
    fclose(file);
}