#pragma once

#include <optional>
#include <stdint.h>

#include "utils.h"

struct DeviceTemp {
    /*1*/ std::optional<int32_t> sensor_1;
    /*2*/ std::optional<int32_t> sensor_2;
    /*3*/ std::optional<int32_t> sensor_3;
    /*4*/ std::optional<int32_t> sensor_4;
    /*5*/ std::optional<int32_t> flex_sensor_1;

    DeviceTemp(picoproto::Message* message) :
        //image_unique_id_low (ToOptional(message->GetUInt64Array(1))),
        sensor_1 (ToOptional(message->GetInt32Array(1))),
        sensor_2 (ToOptional(message->GetInt32Array(2))),
        sensor_3 (ToOptional(message->GetInt32Array(3))),
        sensor_4 (ToOptional(message->GetInt32Array(4))),
        flex_sensor_1 (ToOptional(message->GetInt32Array(5)))
    {}
};

std::ostream& operator<<(std::ostream& os, const DeviceTemp& obj)
{
    os << "{" << std::endl;
    os << "\"sensor_1\": " << obj.sensor_1 << "," << std::endl;
    os << "\"sensor_2\": " << obj.sensor_2 << "," << std::endl;
    os << "\"sensor_3\": " << obj.sensor_3 << "," << std::endl;
    os << "\"sensor_4\": " << obj.sensor_4 << "," << std::endl;
    os << "\"flex_sensor_1\": " << obj.flex_sensor_1 << std::endl;    
    os << "}";
    return os;
}