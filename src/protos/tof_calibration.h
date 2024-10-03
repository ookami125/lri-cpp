#pragma once

#include "../picoproto.h"

struct ToFCalibration {
    /*1*/ float offset_distance;
    /*2*/ float offset_measurement;
    /*3*/ float xtalk_distance;
    /*4*/ float xtalk_measurement;

    ToFCalibration(picoproto::Message* message) :
        offset_distance (message->GetFloat(1)),
        offset_measurement (message->GetFloat(2)),
        xtalk_distance (message->GetFloat(3)),
        xtalk_measurement (message->GetFloat(4))
    {}
};

std::ostream& operator<<(std::ostream& os, const ToFCalibration& obj)
{
    os << "{" << std::endl;
    os << "\"offset_distance\": " << obj.offset_distance << "," << std::endl;
    os << "\"offset_measurement\": " << obj.offset_measurement << "," << std::endl;
    os << "\"xtalk_distance\": " << obj.xtalk_distance << "," << std::endl;
    os << "\"xtalk_measurement\": " << obj.xtalk_measurement << std::endl; 
    return os << "}";
}