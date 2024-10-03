#pragma once
#include <stdint.h>
#include <string.h>

struct Image;

enum class DebayerMode {
    None,
    Filter,
    Interpolate,
};

Image debayerImage(Image* image, uint8_t bayerPatternOffset, DebayerMode mode);