#pragma once
#include <stdint.h>
#include <string.h>

struct Image;

enum class DemosaicMode {
    None,
    Photosite,
    Interpolate,
    WhiteBalance,
};

Image debayerImage(Image* image, uint8_t bayerPatternOffset, DemosaicMode mode);