#pragma once

#include "../picoproto.h"

#include <stdint.h>
#include <vector>
#include <optional>

#include "utils.h"

struct VstModel {
    /*1*/ float a;
    /*2*/ float b;

    VstModel(){}

    VstModel(picoproto::Message* message) : 
        a (message->GetFloat(1)),
        b (message->GetFloat(2))
    {}
};

std::ostream& operator<<(std::ostream& os, const VstModel& obj)
{
    os << "{" << std::endl;
    os << "\"a\": " << obj.a << "," << std::endl;
    os << "\"b\": " << obj.b << std::endl; 
    return os << "}";
}

struct VstNoiseModel {
    /*1*/ uint32_t gain;
    /*2*/ float threshold;
    /*3*/ float scale;
    /*4*/ VstModel red;
    /*5*/ VstModel green;
    /*6*/ VstModel blue;
    /*7*/ std::optional<VstModel> panchromatic;

    VstNoiseModel(picoproto::Message* message) :
        gain ((uint32_t)message->GetUInt64(1)),
        threshold (message->GetFloat(2)),
        scale (message->GetFloat(3)),
        red (message->GetMessage(4)),
        green (message->GetMessage(5)),
        blue (message->GetMessage(6)),
        panchromatic (ToOptional<VstModel>(message->GetMessageArray(7)))
    {}
};

std::ostream& operator<<(std::ostream& os, const VstNoiseModel& obj)
{
    os << "{" << std::endl;
    os << "\"gain\": " << obj.gain << "," << std::endl; 
    os << "\"threshold\": " << obj.threshold << "," << std::endl;
    os << "\"scale\": " << obj.scale << "," << std::endl;
    os << "\"red\": " << obj.red << "," << std::endl;
    os << "\"green\": " << obj.green << "," << std::endl;
    os << "\"blue\": " << obj.blue << "," << std::endl;
    os << "\"panchromatic\": " << obj.panchromatic << std::endl; 
    return os << "}";
}

struct SensorCharacterization {
    /*1*/ float black_level;
    /*2*/ float white_level;
    /*3*/ std::optional<float> cliff_slope;
    /*4*/ std::vector<VstNoiseModel> vst_model;

    SensorCharacterization(picoproto::Message* message) :
        black_level (message->GetFloat(1)),
        white_level (message->GetFloat(2)),
        cliff_slope (ToOptional(message->GetFloatArray(3))),
        vst_model (To<VstNoiseModel>(message->GetMessageArray(4)))
    {}
};

std::ostream& operator<<(std::ostream& os, const SensorCharacterization& obj)
{
    os << "{" << std::endl;
    os << "\"black_level\": " << obj.black_level << "," << std::endl; 
    os << "\"white_level\": " << obj.white_level << "," << std::endl;
    os << "\"cliff_slope\": " << obj.cliff_slope << "," << std::endl;
    os << "\"vst_model\": " << obj.vst_model << std::endl; 
    return os << "}";
}