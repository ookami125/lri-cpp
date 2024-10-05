#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include <vector>
#include <string>
#include <optional>
#include <unordered_map>

#include <stb_image.h>

#include "image.h"
#include "debayer.h"
#include "utils.h"

#include "picoproto.h"
#include "protos/lightheader.h"

struct BitReader {
    const uint8_t* data;
    size_t dataSize;
    size_t bitOffset;
    uint16_t currentValue;

    BitReader(const uint8_t* data, size_t dataSize) 
        : data(data), dataSize(dataSize), bitOffset(0), currentValue(0) {}

    bool getNext10BitValue(uint16_t& value) {
        if (bitOffset + 10 > dataSize * 8) {
            return false;
        }

        size_t byteIndex = bitOffset / 8;
        size_t bitIndex = bitOffset % 8;

        currentValue = 0;

        if (byteIndex < dataSize) {
            currentValue = (data[byteIndex] >> bitIndex) & 0x03FF;
        }
        
        if (byteIndex + 1 < dataSize) {
            currentValue |= ((data[byteIndex + 1] << (8 - bitIndex)) & 0x03FF);
        }

        bitOffset += 10;
        value = currentValue;
        return true;
    }
};

Image unpackToImage(const uint8_t* data, uint32_t width, uint32_t height) {
    BitReader reader = BitReader(data, (width*height*10+9)/16 * 2);

    Image image = Image(width, height, 1);

    for(uint32_t y=0; y<height; ++y) {
        for(uint32_t x=0; x<width; ++x) {
            uint16_t bits;
            if(!reader.getNext10BitValue(bits)) { return image; }
            image.data[x+y*width] = bits;
        }
    }

    return image;
}

enum MessageType : uint8_t {
    LIGHT_HEADER = 0,
    VIEW_PREFERENCES = 1,
    GPS_DATA = 2,
};

#pragma pack(push, 1)
struct LELR_Header {
    char signature[4];
    uint64_t block_length;
    uint64_t message_offset;
    uint32_t message_length;
    MessageType message_type;
    uint8_t padding[7];
};
#pragma pack(pop)

struct Options {
    ImageFileFormat format = ImageFileFormat::PNG;
    std::string outputPath = "buffers";
    DebayerMode debayerMode = DebayerMode::Interpolate;
    std::vector<std::string> positionalArgs;
};

ErrorOr<void> processImage(Options opts)
{
    if(opts.positionalArgs.size() == 0) {
        return {{"LRI File not provided!"}};
    }

    size_t length = 0;
    ErrorOr<char*> eorData = loadFile(opts.positionalArgs[0].c_str(), length);
    if(!eorData.ok()) {
        return eorData.error();
    }
    uint8_t* data = (uint8_t*)eorData.value();

    int surfaceCount = 0;
    
    LELR_Header* header = (LELR_Header*)nullptr;
    for(uint8_t* offset = data; (size_t)(offset - data) < length; offset += header->block_length)
    {
        header = (LELR_Header*)offset;

        if(strncmp(header->signature, "LELR", 4) != 0) break;

        picoproto::Message message;
        message.ParseFromBytes((uint8_t*)header + header->message_offset, header->message_length);
        auto message_ptr = &message;
        
        if(header->message_type == LIGHT_HEADER)
        {
            LightHeader lightHeader = (LightHeader)message_ptr;

            for(auto module : lightHeader.modules)
            {
                Surface surface = module.sensor_data_surface;
                uint8_t* img_data = (uint8_t*)header + surface.data_offset;
                
                const char* surfaceFormats[] = {
                    "BAYER_JPEG", "?", "?", "?", "?", "?", "?", "PACKED_10BPP", "PACKED_12BPP", "PACKED_14BPP",
                };
                
                Image img = Image();

                switch(surface.format)
                {
                    case FormatType::RAW_PACKED_10BPP: {
                        img = unpackToImage(img_data, surface.size.x*2, surface.size.y*2);
                    } break;
                    case FormatType::RAW_BAYER_JPEG: {
					    uint32_t bjpg_header_len = 1576;
					    uint32_t format = *(uint32_t*)(&img_data[4]);

                        printf("BayerJPEG: %d format\n", format);
                        
                        uint32_t jpeg0_len = *(uint32_t*)(&img_data[8]);
					    uint32_t jpeg1_len = *(uint32_t*)(&img_data[12]);
					    uint32_t jpeg2_len = *(uint32_t*)(&img_data[16]);
					    uint32_t jpeg3_len = *(uint32_t*)(&img_data[20]);

                        uint32_t jpeg_len[4] = {
                            jpeg0_len,
                            jpeg1_len,
                            jpeg2_len,
                            jpeg3_len,
                        };

                        uint8_t* jpeg0_offset = &img_data[bjpg_header_len];
                        uint8_t* jpeg1_offset = &jpeg0_offset[jpeg0_len];
                        uint8_t* jpeg2_offset = &jpeg1_offset[jpeg1_len];
                        uint8_t* jpeg3_offset = &jpeg2_offset[jpeg2_len];

                        uint8_t* jpeg_offset[4] = {
                            jpeg0_offset,
                            jpeg1_offset,
                            jpeg2_offset,
                            jpeg3_offset,
                        };

                        uint32_t jpegCount = format ? 1 : 4;
                        for(uint32_t jpegId = 0; jpegId < jpegCount; jpegId++)
                        {
                            char filename_buffer[256];
                            sprintf(filename_buffer, "%s/%02d-%1d-%s.jpg", opts.outputPath.c_str(), surfaceCount, jpegId, surfaceFormats[surface.format]);
                            FILE* file = fopen(filename_buffer, "wb");
                            fwrite(jpeg_offset[jpegId], jpeg_len[jpegId], 1, file);
                            fclose(file);
                        }
                    } break;
                    default: {
                        printf("[%d]Surface Format (%s) not implemented!\n", surfaceCount, surfaceFormats[surface.format]);
                    } break;
                }

                uint8_t bayer = 0;
                if(module.sensor_bayer_red_override.has_value()) {
                    auto bayer_offset = module.sensor_bayer_red_override.value();
                    bayer = (uint8_t)((bayer_offset.x + 2) % 2);
                    bayer |= (uint8_t)(((bayer_offset.y + 2) % 2) << 1);
                }

                if(opts.debayerMode != DebayerMode::None) {
                    img = debayerImage(&img, bayer, opts.debayerMode);
                }

                char filename_buffer[256];
                sprintf(filename_buffer, "%s/%02d-%s.%s", opts.outputPath.c_str(), surfaceCount, surfaceFormats[surface.format], std::to_string(opts.format).c_str());
                img.writeToFile(filename_buffer, opts.format);
                surfaceCount += 1;
            }
        }
    }

    return {};
}

void usage(int argc, char** argv, int exit_code)
{
    const char* progname = "lri_extractor";
    if(argc > 0) {
        std::string path = argv[0];
        progname = path.substr(path.find_last_of("/\\") + 1).c_str();
    }

    printf("Extract the contents of LRI files.\n");
    printf("\n");
    printf("Usage:\n");
    printf("  %s [<options>] lri_file\n", progname);
    printf("  %s -h | --help\n", progname);
    printf("\n");
    printf("Options:\n");
    printf("  -h --help     Show this screen.\n");
    printf("  -f --format   Output format [default: PNG] (PGM,PNG,JPEG)\n");
    printf("  -o --output   Output path [default: \"buffers\"]\n");
    printf("  -d --debayer  Debayering mode [default: Interpolated] (None, Filter, Interpolated)\n");

    std::exit(exit_code);
}

ErrorOr<Options> argparse(int argc, char** argv) {
    Options ret = {};

    for(int argi=1; argi<argc; argi += 1)
    {
        std::string option;
        std::string argument;
        int seperateArgFlag = 0;
        
        if(argv[argi][0] != '-') {
            ret.positionalArgs.push_back(argv[argi]);
            continue;
        }

        if(argv[argi][1] == '-') {
            option = std::string(&argv[argi][2]);
            auto split = option.find('=');
            if(split != option.size()) {
                argument = option.substr(split+1);
                option = option.substr(0, split);
            }
        }
        else
        {
            option = std::string(1, argv[argi][1]);
            if(argv[argi][2] != '\0')
            {
                argument = &argv[argi][2];
            }
        }

        if(argument == "" && argi < argc-1) {
            argument = argv[argi + 1];
            seperateArgFlag = 1;
        }

        if(option == "h" || option == "help") {
            usage(argc, argv, 0);
        } else if(option == "f" || option == "format") {
            const static std::unordered_map<std::string, ImageFileFormat> formatStr = {
                {"pgm", ImageFileFormat::PGM},
                {"png", ImageFileFormat::PNG},
                {"jpg", ImageFileFormat::JPEG},
                {"jpeg", ImageFileFormat::JPEG},
            };
            auto optFormat = formatStr.find(toLower(argument));
            if(optFormat == formatStr.end()) {
                return {{ "Format option (" + argument + ") doesn't exist!"}};
            }
            ret.format = optFormat->second;
            argi += seperateArgFlag;
        } else if(option == "o" || option == "output") {
            ret.outputPath = argument;
            argi += seperateArgFlag;
        } else if(option == "d" || option == "debayer") {
            const static std::unordered_map<std::string, DebayerMode> modeStr = {
                {"none", DebayerMode::None},
                {"filter", DebayerMode::Filter},
                {"interpolate", DebayerMode::Interpolate},
                {"interpolated", DebayerMode::Interpolate},
            };
            auto optMode = modeStr.find(toLower(argument));
            if(optMode == modeStr.end()) {
                return {{ "Error: Format option (" + argument + ") doesn't exist!"}};
            }
            ret.debayerMode = optMode->second;
            argi += seperateArgFlag;
        } else {
            return {{"Error: Unknown option (" + option + ")\n"}};
        }
    }

    return ret;
}

int main(int argc, char** argv) {

    if(argc < 2) {
        usage(argc, argv, 1);
    }

    ErrorOr<Options> options = argparse(argc, argv);
    if(!options.ok()) {
        fprintf(stderr, "Error: %s\n", options.error().msg.c_str());
        usage(argc, argv, 1);
        return 1;
    }

    auto error = processImage(options.value());
    if(!error.ok()) {
        fprintf(stderr, "Error: %s\n", error.error().msg.c_str());
        return 1;
    }

    return 0;
}