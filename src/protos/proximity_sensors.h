#pragma once

#include "../picoproto.h"

struct ProximitySensors {
    /*1*/ bool sensor_1;
    /*2*/ bool sensor_2;
    /*3*/ bool sensor_3;
    /*4*/ bool sensor_4;
    /*5*/ bool sensor_5;

    ProximitySensors(picoproto::Message* message) :
        sensor_1 (message->GetBool(1)),
        sensor_2 (message->GetBool(2)),
        sensor_3 (message->GetBool(3)),
        sensor_4 (message->GetBool(4)),
        sensor_5 (message->GetBool(5))
    {}
};

std::ostream& operator<<(std::ostream& os, const ProximitySensors& obj)
{
    os << "{" << std::endl;
    os << "\"sensor_1\": " << obj.sensor_1 << "," << std::endl; 
    os << "\"sensor_2\": " << obj.sensor_2 << "," << std::endl;
    os << "\"sensor_3\": " << obj.sensor_3 << "," << std::endl;
    os << "\"sensor_4\": " << obj.sensor_4 << "," << std::endl;
    os << "\"sensor_5\": " << obj.sensor_5 << std::endl; 
    return os << "}";
}