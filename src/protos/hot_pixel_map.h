#pragma once

#include "../picoproto.h"

#include <optional>
#include <stdint.h>
#include <vector>

struct HotPixelMeasurement {
    /*1*/ uint64_t data_offset;
    /*2*/ uint32_t data_size;
    /*3*/ uint32_t data_exposure;
    /*4*/ int32_t sensor_temparature;
    /*5*/ float sensor_gain;
    /*6*/ std::optional<float> pixel_variance;
    /*7*/ std::optional<float> threshold;

    HotPixelMeasurement(picoproto::Message* message) :
        data_offset (message->GetUInt64(1)),
        data_size (message->GetUInt32(2)),
        data_exposure (message->GetUInt32(3)),
        sensor_temparature (message->GetInt32(4)),
        sensor_gain (message->GetFloat(5)),
        pixel_variance (ToOptional(message->GetFloatArray(6))),
        threshold (ToOptional(message->GetFloatArray(7)))
    {}
};

std::ostream& operator<<(std::ostream& os, const HotPixelMeasurement& obj)
{
    os << "{" << std::endl;
    os << "\"data_offset\": " << obj.data_offset << "," << std::endl; 
    os << "\"data_size\": " << obj.data_size << "," << std::endl;
    os << "\"data_exposure\": " << obj.data_exposure << "," << std::endl;
    os << "\"sensor_temparature\": " << obj.sensor_temparature << "," << std::endl;
    os << "\"sensor_gain\": " << obj.sensor_gain << "," << std::endl;
    os << "\"pixel_variance\": " << obj.pixel_variance << "," << std::endl;
    os << "\"threshold\": " << obj.threshold << std::endl; 
    return os << "}";
}

struct HotPixelMap {
    /*1*/ std::vector<HotPixelMeasurement> data;

    HotPixelMap(picoproto::Message* message) :
        data (To<HotPixelMeasurement>(message->GetMessageArray(1)))
    {}
};

std::ostream& operator<<(std::ostream& os, const HotPixelMap& obj)
{
    os << "{" << std::endl;
    os << "\"data\": " << obj.data << std::endl; 
    return os << "}";
}