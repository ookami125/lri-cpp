#pragma once
#include <stdint.h>
#include <string.h>
#include <vector>
#include <unordered_map>
#include "error.h"

enum class ImageFileFormat {
    PGM,
    JPEG,
    PNG,
};

const static std::unordered_map<ImageFileFormat, std::string> imageFileFormatToString = {
    {ImageFileFormat::PGM, "pgm"},
    {ImageFileFormat::PNG, "png"},
    {ImageFileFormat::JPEG, "jpeg"},
};

namespace std {
    inline std::string to_string(ImageFileFormat val) {
        auto pair = imageFileFormatToString.find(val);
        if(pair != imageFileFormatToString.end()) {
            return pair->second;
        }
        return "<unknown>";
    }
};


struct Image {
    std::vector<uint16_t> data;
    uint32_t width, height;
    uint8_t channels;

    Image() = default;
    Image(uint32_t width, uint32_t height, uint8_t channels);
    ~Image();

    uint16_t getColor(int x, int y, int c);
    uint16_t getColorOrMirror(int x, int y, int c);
    void setColor(int x, int y, int chan, uint16_t color);

    ErrorOr<void> writeToFile(const char* path, ImageFileFormat format);
};