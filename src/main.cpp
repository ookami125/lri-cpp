#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cstring>
#include <vector>
#include <string>
#include <optional>
#include <unordered_map>

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

Image unpackToImage(const uint8_t* data, size_t width, size_t height, uint8_t bayerPattern) {
    BitReader reader = BitReader(data, (width*height*10+9)/16 * 2);

    Image image = Image(width, height, 1);

    for(int y=0; y<height; ++y) {
        for(int x=0; x<width; ++x) {
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

struct LELR_Header {
    char signature[4];
    uint64_t block_length;
    uint64_t message_offset;
    uint32_t message_length;
    MessageType message_type;
    uint8_t padding[7];
} __attribute__((packed));

struct Options {
    ImageFileFormat format = ImageFileFormat::PGM;
    std::string outputPath = "buffers";
    DebayerMode debayerMode = DebayerMode::Interpolate;
    std::vector<std::string> positionalArgs;
};

void processImage(Options opts)
{
    if(opts.positionalArgs.size() == 0) {
        printf("Error: LRI File not provided!");
        std::exit(1);
    }

    size_t length = 0;
    uint8_t* data = (uint8_t*)loadFile(opts.positionalArgs[0].c_str(), length);

    int surfaceCount = 0;
    
    LELR_Header* header = (LELR_Header*)nullptr;
    for(uint8_t* offset = data; offset - data < length; offset += header->block_length)
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
                uint8_t* data_offset = (uint8_t*)header + surface.data_offset;
                
                const char* surfaceFormats[] = {
                    "BAYER_JPEG", "?", "?", "?", "?", "?", "?", "PACKED_10BPP", "PACKED_12BPP", "PACKED_14BPP",
                };
                
                if(surface.format == FormatType::RAW_PACKED_10BPP)
                {
                    size_t length = surface.size.y * surface.row_stride;
                    
                    uint8_t bayer = 0;
                    if(module.sensor_bayer_red_override.has_value()) {
                        auto bayer_offset = module.sensor_bayer_red_override.value();
                        bayer = (bayer_offset.x + 2) % 2;
                        bayer |= ((bayer_offset.y + 2) % 2) << 1;
                    }

                    char filename_buffer[256];
                    sprintf(filename_buffer, "%s/%02d-%s.pgm", opts.outputPath.c_str(), surfaceCount, surfaceFormats[surface.format]);
                    Image img = unpackToImage(data_offset, 4160, 3120, bayer);

                    if(opts.debayerMode != DebayerMode::None) {
                        img = debayerImage(&img, bayer, opts.debayerMode);
                    }

                    img.writeToFile(filename_buffer, opts.format);
                }
                else {
                    printf("[%s ]Surface Format (%s) not implemented!\n", surfaceCount, surfaceFormats[surface.format]);
                }
                surfaceCount += 1;
            }
        }
    }
}

void usage(int argc, char** argv, int exit_code)
{
    const char* progname = "lri_extractor";
    if(argc > 0) {
        progname = basename(argv[0]);
    }

    printf("Extract the contents of LRI files.\n");
    printf("\n");
    printf("Usage:\n");
    printf("  %s [<options>] lri_file\n", progname);
    printf("  %s -h | --help\n", progname);
    printf("\n");
    printf("Options:\n");
    printf("  -h --help     Show this screen.\n");
    printf("  -f --format   Output format [default: PGM] (PGM)\n");
    printf("  -o --output   Output path [default: \"buffers\"]\n");
    printf("  -d --debayer  Debayering mode [default: Interleaved] (None, Filter, Interleaved)\n");

    std::exit(exit_code);
}

int main(int argc, char** argv) {

    if(argc < 2) {
        usage(argc, argv, 1);
    }

    Options options;

    for(int argi=1; argi<argc; argi += 1) {
        std::string option;
        std::string argument;
        
        if(argv[argi][0] != '-') {
            options.positionalArgs.push_back(argv[argi]);
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
            argi += 1;
            argument = argv[argi];
        }

        if(option == "h" || option == "help") {
            usage(argc, argv, 0);
        } else if(option == "f" || option == "format") {
            const static std::unordered_map<std::string, ImageFileFormat> formatStr = {
                {"pgm", ImageFileFormat::PGM},
            };
            auto optFormat = formatStr.find(toLower(argument));
            if(optFormat == formatStr.end()) {
                printf("Error: Format option %s doesn't exist!", argument.c_str());
                std::exit(1);
            }
            options.format = optFormat->second;
        } else if(option == "o" || option == "output") {
            options.outputPath = option;
        } else  if(option == "d" || option == "debayer") {
            const static std::unordered_map<std::string, DebayerMode> modeStr = {
                {"none", DebayerMode::None},
                {"filter", DebayerMode::Filter},
                {"interpolate", DebayerMode::Interpolate},
            };
            auto optMode = modeStr.find(toLower(argument));
            if(optMode == modeStr.end()) {
                printf("Error: Format option %s doesn't exist!", argument.c_str());
                std::exit(1);
            }
            options.debayerMode = optMode->second;
        } else {
            printf("Error: Unknown option (%s)\n", option.c_str());
            usage(argc, argv, 1);
        }
    }

    processImage(options);

    return 0;
}