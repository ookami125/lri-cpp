#include "image.h"
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include "error.h"

#include <stb_image_write.h>

Image::Image(uint32_t width, uint32_t height, uint8_t channels) :
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
    int xc = clamp(x, 0, (int32_t)width-1);
    int yc = clamp(y, 0, (int32_t)height-1);
    return data[(size_t)((xc+yc*(int32_t)width)*channels+c)];
};

uint16_t Image::getColorOrMirror(int x, int y, int c) {
    const int32_t w = (int32_t)width-1;
    const int32_t h = (int32_t)height-1;
    // Stupid math to mirror the image
    x = w-abs(abs(x)%(w*2)-w);
    y = h-abs(abs(y)%(h*2)-h);
    return data[(size_t)((x+y*(int32_t)width)*channels+c)];
};

void Image::setColor(int x, int y, int chan, uint16_t color) {
    int xc = clamp(x, 0, (int32_t)width-1);
    int yc = clamp(y, 0, (int32_t)height-1);
    data[(size_t)((xc+yc*(int32_t)width)*channels+chan)] = color;
}

ErrorOr<void> Image::writeToFile(const char* path, ImageFileFormat format) {
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
                return {{"invalid number of channels for PGM file format!"}};
            }

            fprintf(file, "%d %d\n%d\n", width, height, max);

            for(uint32_t y=0; y<height; ++y) {
                for(uint32_t x=0; x<width; ++x) {
                    uint16_t* pixel = &data[(x+y*width) * channels];
                    for(int channel=0; channel<channels; channel++) {
                        fprintf(file, "%d ", pixel[channel]);
                    }
                }
                fprintf(file, "\n");
            }
        } break;
        case ImageFileFormat::PNG: {
            std::vector<uint8_t> converted = {};
            converted.reserve(width*height*channels);
            for(uint16_t c : data) {
                converted.push_back((uint8_t)(c >> 2));
            }

            int res = stbi_write_png(path, (int)width, (int)height, channels, converted.data(), (int)(width*channels));
            if(res == 0) {
                fclose(file);
                return {{"Failed to write PNG ("+std::string(path)+")"}};
            }
        } break;
        case ImageFileFormat::JPEG: {
            std::vector<uint8_t> converted = {};
            converted.reserve(width*height*channels);
            for(uint16_t c : data) {
                converted.push_back((uint8_t)(c >> 2));
            }

            int res = stbi_write_jpg(path, (int)width, (int)height, channels, converted.data(), 100);
            if(res == 0) {
                fclose(file);
                return {{"Failed to write JPG ("+std::string(path)+")"}};
            }
        } break;
        default: {
            return {{"Unimplemented ImageFileFormat ("+std::to_string((uint32_t)format)+")"}};
        }
    }
    fclose(file);
    
    return {};
}