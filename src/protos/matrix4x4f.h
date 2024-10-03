#pragma once

#include "../picoproto.h"

#include <stdint.h>

struct Matrix4x4F {
    /*1*/ float x00;
    /*2*/ float x01;
    /*3*/ float x02;
    /*4*/ float x03;
    /*5*/ float x10;
    /*6*/ float x11;
    /*7*/ float x12;
    /*8*/ float x13;
    /*9*/ float x20;
    /*10*/ float x21;
    /*11*/ float x22;
    /*12*/ float x23;
    /*13*/ float x30;
    /*14*/ float x31;
    /*15*/ float x32;
    /*16*/ float x33;
    
    Matrix4x4F(){}

    Matrix4x4F(picoproto::Message* message) : 
        x00 (message->GetFloat(1)),
        x01 (message->GetFloat(2)),
        x02 (message->GetFloat(3)),
        x03 (message->GetFloat(4)),
        x10 (message->GetFloat(5)),
        x11 (message->GetFloat(6)),
        x12 (message->GetFloat(7)),
        x13 (message->GetFloat(8)),
        x20 (message->GetFloat(9)),
        x21 (message->GetFloat(10)),
        x22 (message->GetFloat(11)),
        x23 (message->GetFloat(12)),
        x30 (message->GetFloat(13)),
        x31 (message->GetFloat(14)),
        x32 (message->GetFloat(15)),
        x33 (message->GetFloat(16))
    {}
};

std::ostream& operator<<(std::ostream& os, const Matrix4x4F& obj)
{
    os << "[" << std::endl;
    os << obj.x00 << ",";
    os << obj.x01 << ",";
    os << obj.x02 << ",";
    os << obj.x03 << "," << std::endl;
    os << obj.x10 << ",";
    os << obj.x11 << ",";
    os << obj.x12 << ",";
    os << obj.x13 << "," << std::endl;
    os << obj.x20 << ",";
    os << obj.x21 << ",";
    os << obj.x22 << ",";
    os << obj.x23 << "," << std::endl;
    os << obj.x30 << ",";
    os << obj.x31 << ",";
    os << obj.x32 << ",";
    os << obj.x33 << std::endl;
    return os << "]";
}