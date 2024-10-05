#pragma once

#include "../picoproto.h"

#include "point3f.h"

#include "utils.h"

struct Sample {
    /*1*/ uint32_t row_idx;
    /*2*/ Point3F data;

    Sample(picoproto::Message* message) :
        row_idx ((uint32_t)message->GetUInt64(1)),
        data (message->GetMessage(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const Sample& obj)
{
    os << "{" << std::endl;
    os << "\"row_idx\": " << obj.row_idx << "," << std::endl; 
    os << "\"data\": " << obj.data << "," << std::endl;
    return os << "}";
}

struct IMUData {
    /*1*/ uint32_t frame_index;
    /*2*/ std::vector<Sample> accelerometer;
    /*3*/ std::vector<Sample> gyroscope;

    IMUData(picoproto::Message* message) :
        frame_index ((uint32_t)message->GetUInt64(1)),
        accelerometer (To<Sample>(message->GetMessageArray(2))),
        gyroscope (To<Sample>(message->GetMessageArray(2)))
    {}
};

std::ostream& operator<<(std::ostream& os, const IMUData& obj)
{
    os << "{" << std::endl;
    os << "\"frame_index\": " << obj.frame_index << "," << std::endl; 
    os << "\"accelerometer\": " << obj.accelerometer << "," << std::endl;
    os << "\"gyroscope\": " << obj.gyroscope << std::endl; 
    return os << "}";
}