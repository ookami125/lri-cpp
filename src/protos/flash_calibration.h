#pragma once

#include "../picoproto.h"

struct FlashCalibration {
    /*1*/ float ledcool_lux;
    /*2*/ float ledcool_max_lumens;
    /*3*/ float ledcool_cct;
    /*4*/ float ledwarm_lux;
    /*5*/ float ledwarm_max_lumens;
    /*6*/ float ledwarm_cct;

    FlashCalibration(picoproto::Message* message) :
        ledcool_lux (message->GetFloat(1)),
        ledcool_max_lumens (message->GetFloat(2)),
        ledcool_cct (message->GetFloat(3)),
        ledwarm_lux (message->GetFloat(4)),
        ledwarm_max_lumens (message->GetFloat(5)),
        ledwarm_cct (message->GetFloat(6))
    {}
};

std::ostream& operator<<(std::ostream& os, const FlashCalibration& obj)
{
    os << "{" << std::endl;
    os << "\"ledcool_lux\": " << obj.ledcool_lux << "," << std::endl; 
    os << "\"ledcool_max_lumens\": " << obj.ledcool_max_lumens << "," << std::endl;
    os << "\"ledcool_cct\": " << obj.ledcool_cct << "," << std::endl;
    os << "\"ledwarm_lux\": " << obj.ledwarm_lux << "," << std::endl;
    os << "\"ledwarm_max_lumens\": " << obj.ledwarm_max_lumens << "," << std::endl;
    os << "\"ledwarm_cct\": " << obj.ledwarm_cct << std::endl; 
    return os << "}";
}