#pragma once
#include <stdint.h>
#include <string.h>
#include <vector>
#include "error.h"

enum class ImageFileFormat {
    PGM
};

struct Image {
    std::vector<uint16_t> data;
    uint32_t width, height;
    uint8_t channels;

    Image(uint32_t width, uint32_t height, uint8_t channels);
    ~Image();

    uint16_t getColor(int x, int y, int c);
    uint16_t getColorOrMirror(int x, int y, int c);
    void setColor(int x, int y, int chan, uint16_t color);

    ErrorOr<void> writeToFile(const char* path, ImageFileFormat format);
};