#pragma once

#include "../picoproto.h"

#include <stdint.h>
#include <iostream>

struct Point3F {
    /*1*/ float x;
    /*2*/ float y;
    /*3*/ float z;
    
    Point3F(){}

    Point3F(picoproto::Message* message) : 
        x (message->GetFloat(1)),
        y (message->GetFloat(2)),
        z (message->GetFloat(3))
    {}
};


std::ostream& operator<<(std::ostream& os, const Point3F& obj)
{
    return os << "{ \"x\":" << obj.x << ", \"y\":" << obj.y << ", \"z\":" << obj.z << " }";
}