#include "debayer.h"
#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include "utils.h"

uint16_t average(uint16_t lhs, uint16_t rhs) {
    return (uint16_t)((lhs >> 1) + (rhs >> 1u) + (lhs & rhs & 1));
}

Image debayerImage(Image* srcImage, uint8_t bayerPatternOffset, DebayerMode mode) {

    const int R=0, G=1, B=2;
    char map[] = { R,G,G,B };
    if(bayerPatternOffset & 0x1) {
        swap(map[0], map[1]);
        swap(map[2], map[3]);
    }
    if(bayerPatternOffset & 0x2) {
        swap(map[0], map[2]);
        swap(map[1], map[3]);
    }

    // Pull out RGB values into their seperate channels
    Image image = Image(srcImage->width, srcImage->height, 3);
    for(int32_t y=0; y<(int32_t)srcImage->height; ++y) {
        for(int32_t x=0; x<(int32_t)srcImage->width; ++x) {
            char id = map[((y%2)<<1) | x%2];
            image.setColor(x, y, id, srcImage->getColor(x, y, 0));
        }
    }

    switch(mode) {
        default:
        case DebayerMode::Filter:
            break;

        case DebayerMode::Interpolate: {
            //Interpolate RGB values to other pixels
            Image debayeredImage = image;
            for(int32_t y=0; y<(int64_t)image.height; y++) {
                for(int32_t x=0; x<(int64_t)image.width; x++) {
                    char id = map[((y%2)<<1) | x % 2];
                    switch(id) {
                        case R: {
                            uint16_t g = average(
                                average(image.getColorOrMirror(x-1, y, 1), image.getColorOrMirror(x, y-1, 1)),
                                average(image.getColorOrMirror(x+1, y, 1), image.getColorOrMirror(x, y+1, 1)));
                            debayeredImage.setColor(x, y, 1, g);
                            uint16_t b = average(
                                average(image.getColorOrMirror(x-1, y-1, 2), image.getColorOrMirror(x-1, y+1, 2)),
                                average(image.getColorOrMirror(x+1, y-1, 2), image.getColorOrMirror(x+1, y+1, 2)));
                            debayeredImage.setColor(x, y, 2, b);
                        } break;
                        case G: {
                            uint16_t r = average(
                                image.getColorOrMirror(x-1, y, 0) + image.getColorOrMirror(x, y-1, 0),
                                image.getColorOrMirror(x+1, y, 0) + image.getColorOrMirror(x, y+1, 0));
                            debayeredImage.setColor(x, y, 0, r);
                            uint16_t b = average(
                                image.getColorOrMirror(x-1, y, 2) + image.getColorOrMirror(x, y-1, 2),
                                image.getColorOrMirror(x+1, y, 2) + image.getColorOrMirror(x, y+1, 2));
                            debayeredImage.setColor(x, y, 2, b);
                        } break;
                        case B: {
                            uint16_t r = average(
                                average(image.getColorOrMirror(x-1, y-1, 0), image.getColorOrMirror(x-1, y+1, 0)),
                                average(image.getColorOrMirror(x+1, y-1, 0), image.getColorOrMirror(x+1, y+1, 0)));
                            debayeredImage.setColor(x, y, 0, r);
                            uint16_t g = average(
                                average(image.getColorOrMirror(x-1, y, 1), image.getColorOrMirror(x, y-1, 1)),
                                average(image.getColorOrMirror(x+1, y, 1), image.getColorOrMirror(x, y+1, 1)));
                            debayeredImage.setColor(x, y, 1, g);
                        } break;
                    }
                }
            }
            image = std::move(debayeredImage);
        } break;

    }

    return image;
}