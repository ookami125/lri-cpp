#pragma once

#include "../picoproto.h"

#include <stdint.h>

struct Matrix3x3F {
    /*1*/ float x00;
    /*2*/ float x01;
    /*3*/ float x02;
    /*4*/ float x10;
    /*5*/ float x11;
    /*6*/ float x12;
    /*7*/ float x20;
    /*8*/ float x21;
    /*9*/ float x22;
    
    Matrix3x3F(){}

    Matrix3x3F(picoproto::Message* message) : 
        x00 (message->GetFloat(1)),
        x01 (message->GetFloat(2)),
        x02 (message->GetFloat(3)),
        x10 (message->GetFloat(4)),
        x11 (message->GetFloat(5)),
        x12 (message->GetFloat(6)),
        x20 (message->GetFloat(7)),
        x21 (message->GetFloat(8)),
        x22 (message->GetFloat(9))
    {}
};

std::ostream& operator<<(std::ostream& os, const Matrix3x3F& obj)
{
    os << "[" << std::endl;
    os << obj.x00 << ",";
    os << obj.x01 << ",";
    os << obj.x02 << "," << std::endl;
    os << obj.x10 << ",";
    os << obj.x11 << ",";
    os << obj.x12 << "," << std::endl;
    os << obj.x20 << ",";
    os << obj.x21 << ",";
    os << obj.x22 << std::endl;
    return os << "]";
}