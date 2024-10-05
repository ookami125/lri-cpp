#pragma once

#include "../picoproto.h"

#include <stdint.h>
#include <optional>

#include "utils.h"

struct DeadPixelMap {
    /*1*/ uint64_t data_offset;
    /*2*/ uint32_t data_size;
    /*3*/ std::optional<float> black_level_threshold;

    DeadPixelMap(picoproto::Message* message) :
        data_offset (message->GetUInt64(1)),
        data_size ((uint32_t)message->GetUInt64(2)),
        black_level_threshold (ToOptional(message->GetFloatArray(3)))
    {}
};

std::ostream& operator<<(std::ostream& os, const DeadPixelMap& obj)
{
    os << "{" << std::endl;
    os << "\"data_offset\": " << obj.data_offset << "," << std::endl; 
    os << "\"data_size\": " << obj.data_size << "," << std::endl;
    os << "\"black_level_threshold\": " << obj.black_level_threshold << std::endl; 
    return os << "}";
}